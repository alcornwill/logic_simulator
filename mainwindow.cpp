#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QGraphicsTextItem>
#include "component.h"
#include "mygraphicsscene.h"
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include "connection.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include <QPointF>
#include <QUndoStack>
#include "deleteconnectioncommand.h"
#include "deletecommand.h"
#include <QButtonGroup>
#include <QLayout>
#include <QClipboard>
#include <QList>
#include <QFileSystemModel>
#include <QMutableListIterator>
#include "createconnectioncommand.h"
#include "movecommand.h"
#include "bitinputterminal.h"
#include "byteinputterminal.h"
#include "bitoutputterminal.h"
#include "byteoutputterminal.h"
#include "bitconnection.h"
#include "byteconnection.h"
#include "connectiontype.h"
#include "myresources.h"
#include "connectionnode.h"
#include "blockpanel.h"
#include <QSettings>
#include <QGraphicsSceneWheelEvent>
#include <QEvent>
#include "lua.hpp"

void MainWindow::SubStep()
{
    for (int i=0; i < myscene->components.size(); i++)
    {
        Component * component = myscene->components[i];
        Q_ASSERT(component); // anything can happen
        component->Reevaluate();
    }

    // reset combinational components
    for (int i=0; i < myscene->components.size(); i++)
    {
        Component * component = myscene->components[i];
        if (component->combinational)
            component->reevaluated = false;
    }
}

void MainWindow::Reevaluate()
{
    // do the reevaluation thing
    // each reevaluation period, for each component
    // it's inputs are recursively reevaluated and then reevaluates it's outputs

    SubStep();
    SubStep();
    SubStep(); // doing this 3 times reduces likelyhood of getting race conditions

    // after reevaluation is done, reset all components
    for (int i=0; i < myscene->components.size(); i++)
    {
        Component * component = myscene->components[i];
        component->reevaluated = false;
    }
}

MainWindow * MainWindow::mainWindow = 0;

int MainWindow::lua_createComponent(lua_State *luaState)
{
    int type = lua_tonumber(luaState, 1);
    int posX = lua_tonumber(luaState, 2);
    int posY = lua_tonumber(luaState, 3);
    int ID = lua_tonumber(luaState, 4);
    int parentID = lua_tonumber(luaState, 5);

    mainWindow->createComponent(type, posX, posY, ID, parentID);
    return 0;
}


int MainWindow::lua_changeComponentSettings(lua_State *luaState)
{
    int ID = lua_tointeger(luaState, 1);
    const char * settings = lua_tostring(luaState, 2);
    mainWindow->changeComponentSettings(ID, settings);
    return 0;
}

// wanted to make this generic but it wasn't as simple as I thought
// this will mess up the stack so read any other arguments you want first
// I guess that means you can't have two table arguments. what a load of shit
QList<int> tableToQListInt(lua_State * L, int index)
{
    QList<int> list;

    // whoever made this API was an asshole
    lua_pushnil(L);  // is read by lua_next, makes it read index 1
    while (lua_next(L, index) != 0) {
        // lua_pop(L, -3); // pop the lua_next index
        list << lua_tointeger(L, -1); // read the value. key is at index -2

        // remove 'value' from the stack.
        lua_pop(L, 1);
        // don't remove key so we can reuse it for lua_next (because lua arrays are 1-indexed) (this wasn't my idea)
    }

    return list;
}

int MainWindow::lua_createConnection(lua_State *luaState)
{
    const int outputId = lua_tointeger(luaState, 1);
    const int inputId = lua_tointeger(luaState, 2);
    const char * outputName = lua_tostring(luaState, 3);
    const char * inputName = lua_tostring(luaState, 4);
    const int type = lua_tointeger(luaState, 5);
    QList<int> nodes = tableToQListInt(luaState, 6);

    mainWindow->deserializeConnection(outputName,inputId,outputId,inputName,nodes,type);
    return 0;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mywindow = this;
    mainWindow = this;
    myresources = new MyResources();

    // initialize assembly stuff
    // initialize allowed chars
    for (int i = 1; i <= 8; i++)
        allowedChars << i;
    for (int i = 24; i <= 127; i++)
        allowedChars << i;
    for (int i = 160; i <= 255; i++)
        allowedChars << i;
    // doesn't work :(
//    allowedChars << 9; // tab
//    allowedChars << 13; // carriage return

    // initialize instructions
    // it's "inspired" by the MIPS instruction set

    instructions << "NOP"; // 0 (no op)

    // logic arithmetic
    instructions << "ADD"; // 1
    instructions << "SUB"; // 2
    instructions << "DIV"; // 3
    instructions << "MUL"; // 4
    instructions << "SL";  // 5
    instructions << "SR";  // 6
    instructions << "AND"; // 7
    instructions << "OR";  // 8
    instructions << "NOT"; // 9 (not an instruction in real life apparently)
    instructions << "NOR"; // 10
    instructions << "XOR"; // 11

    // control structure
    instructions << "BEQ"; // 12 (not implemented)
    instructions << "BNE"; // 13 (not implemented)
    instructions << "BLS"; // 14 (not implemented)(in MIPS this means "branch if less than zero" but mine just means "branch if x < y")
    instructions << "BGT"; // 15 (not implemented)
    instructions << "J";   // 16
    instructions << "JAL"; // 17 (coming soon)
    instructions << "JR";  // 18 (coming soon)

    // memory operations
    instructions << "L";   // 19
    instructions << "LI";  // 20
    instructions << "S";   // 21
    instructions << "SI";  // 22
    instructions << "MOV"; // 23


    // initialize scene
    myscene = new MyGraphicsScene(this);
    ui->graphicsView->setScene(myscene);
    ui->graphicsView->viewport()->setMouseTracking(true);

    // start timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Reevaluate()));
    timer->start(simulationFrequency);

    // do this
    ui->graphicsView->viewport()->installEventFilter(this);

    // add buttons to group
    buttonGroup.addButton(ui->OrGateToolButton);
    buttonGroup.addButton(ui->AndGateToolButton);
    buttonGroup.addButton(ui->ClockToolButton);
    buttonGroup.addButton(ui->BlockToolButton);
    buttonGroup.addButton(ui->NotGateToolButton);
    buttonGroup.addButton(ui->AdderToolButton);
    buttonGroup.addButton(ui->InputToolButton);
    buttonGroup.addButton(ui->OutputToolButton);
    buttonGroup.addButton(ui->MemoryToolButton);
    buttonGroup.addButton(ui->RandomToolButton);
    buttonGroup.addButton(ui->FlipFlopToolButton);
    buttonGroup.addButton(ui->CounterToolButton);
    buttonGroup.addButton(ui->MuxToolButton);
    buttonGroup.addButton(ui->UpToolButton);
    buttonGroup.addButton(ui->DownToolButton);
    buttonGroup.addButton(ui->NandToolButton);
    buttonGroup.addButton(ui->NorToolButton);
    buttonGroup.addButton(ui->XorToolButton);
    buttonGroup.addButton(ui->ControlToolButton);
    buttonGroup.addButton(ui->EqualityToolButton);
    buttonGroup.addButton(ui->DelayToolButton);
    buttonGroup.addButton(ui->DemuxToolButton);

    // add an extra invisible "none" button
    noneButton.setVisible(false);
    noneButton.setCheckable(true);
    buttonGroup.addButton(&noneButton);

    // why
    ui->verticalLayout_4->setAlignment(Qt::AlignTop);

    // do this as well
    QObject::connect(myscene->undoStack, SIGNAL(indexChanged(int)), this, SLOT(undoIndexChanged(int)));

    // initialize the file explorer
    model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());
    model->setNameFilterDisables(false);
    model->setNameFilters(QStringList() << "*.net");
    ui->treeView->setModel(model);

    loadUserSettings();

    // initialize lua
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "createComponent", lua_createComponent);
    lua_register(L, "changeComponentSettings", lua_changeComponentSettings);
    lua_register(L, "createConnection", lua_createConnection);

    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    lua_close(L);

    saveUserSettings();

    mywindow = 0;

    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->graphicsView->viewport() && event->type() == QEvent::Wheel)
    {
        return true;
    }
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers().testFlag(Qt::AltModifier))
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    else
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::RubberBandDrag);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers().testFlag(Qt::AltModifier))
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
    else
        ui->graphicsView->setDragMode(QGraphicsView::DragMode::RubberBandDrag);
}

// deletes all child widgets or layout items of layout
void MainWindow::deleteAll(QLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0)
    {
        if (child->widget()) {
            layout->removeWidget(child->widget());
            delete child->widget();
        } else {
            layout->removeItem(child);
        }
        delete child;
    }
}

int MainWindow::newComponentId()
{
    // hmm, looking at serialized nets, this obviously doesn't work properly, all IDs are odd!

    // iterates through list of components and finds the next free id number
    // starting at nextFreeComponentId
    // it should work because the only time the nextFreeComponentId can decrease,
    // is when we remove a component, which I've got covered in MainWindow::RemoveComponent
    // oh, and whenever a new net is opened

    // this is probably the stupidest most inefficient thing ever
    // should use hashtable but cba because refactoring sucks
    while (1)
    {
        if (myscene->findComponentWithId(nextFreeComponentId))
        {
            nextFreeComponentId++;
            continue;
        }

        // made it to end: nextFreeComponentId really is free
        break;
    }

    return nextFreeComponentId;
}

// these methods are only going to be called by undocommands
void MainWindow::RemoveComponent(Component * component, bool undoable)
{
    component->deleted = true;

    // this removes a component from the scene, but doesn't delete it from memory

    // remove childComponent reference in parent
    if (component->parentComponent)
        component->parentComponent->removeChildComponent(component);

    // remove our children from the scene too (recursive)
    // we need an iterator since they will start dissapearing from the list
    QMutableListIterator<Component*> iter(component->childComponents);
    while (iter.hasNext())
        RemoveComponent(iter.next());

    if (nextFreeComponentId > component->componentID)
        nextFreeComponentId = component->componentID;

    // we also need to remove any connections that are attached to us
    // *by accident* these commands are automatically put into a macro when deleting components
    if (undoable)
        component->deleteAllConnections();
    // if not undoable then connections should be deleted by destructors/QT's memory management system

    if (component->scene())
        myscene->removeItem(component); // this will remove all descentants from the scene
    myscene->components.removeOne(component);
}

void MainWindow::AddComponent(Component * component, Component * parentComponent)
{
    // this adds a component that has already been created back to the scene

    component->deleted = false;
    myscene->addItem(component); // the docs say not to call this but they don't understand
    if (parentComponent) parentComponent->setChildComponent(component);
    // hmm, this failed one time. wonder why
    Q_ASSERT(!myscene->findComponentWithId(component->componentID)); // hopefully this isn't possible
    myscene->components << component;
}

void MainWindow::loadUserSettings()
{
    // loads user settings from .ini file
    // you could save window layout, apparently it's easy with QSettings

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();
}

void MainWindow::saveUserSettings()
{
    // saves .user settings to install location (working directory?)

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
}


QList<Component *> MainWindow::getChildrenRecursive(Component *component)
{
    QList<Component *> descendants;
    for (int i = 0; i < component->childComponents.size(); i++)
    {
        Component * descendant = component->childComponents[i];
        descendants << descendant; // add self first is very important!
        descendants << getChildrenRecursive(descendant);
    }

    return descendants;
}

QString MainWindow::serializeComponents(QList<Component *> toSerialize)
{
    // this creates lua code
    QMutableListIterator<Component*> iter(toSerialize);
    while (iter.hasNext())
    {
        Component * component = iter.next();
        if (component->parentComponent)
        {
            if (anyParentsInList(component,toSerialize))
                // if a parent *is* being serialized,
                // remove this descentant from toSerialize as it will get added again by getChildrenRecursive
                // actually, this is really useful because it also means when we deserialize the component,
                // we can guarentee that it's parent already exists
                iter.remove();
            else
                // if you're trying to serialize descentant without saving parent
                // it basically wouldn't make sense when you paste it, so
                // make it a top level item
                // (the position will be kind of random, but I like it like that)
                component->parentComponent->removeChildComponent(component);
        }
    }    

    QList<Component *> descendants;
    // add all descendants of toSerialize to list, because that's how we'd expect it to work
    // (toSerialize should only contain top level components at this point)
    for (int i = 0; i < toSerialize.size(); i++)
        descendants << getChildrenRecursive(toSerialize[i]);

    // add descendants to toSerialize
    toSerialize << descendants;

    QTextStream stream(&QString());
    // stream << header << "\n"; removing this for lua
    QList<Connection*> connections;

    for (int i=0; i < toSerialize.size(); i++)
    {
        // for each component, save the type, position and ID
        Component * component = toSerialize[i];
        if (component->type != ComponentType::tBlockNode)
        {
            // serialize the component
            // don't serialize block nodes, but we still want the connections
            // block component will save the block node IDs and deserialize them (it's horrible)
            stream << "createComponent(";
            QPointF position = component->pos();
            stream << component->type << ","
                   << position.x() << ","
                   << position.y() << ","
                   << component->componentID << ",";
            if (component->parentComponent)
                stream << component->parentComponent->componentID;
            else
                stream << "-1";
            stream << ")\n";

            // the next line is the component settings
            stream << "changeComponentSettings(" << component->componentID << ",";
            stream << "\"" << component->SerializeSettings() + "\")\n";
        }

        // get the connections (we'll save them later)
        // if the connection is not a child of an output terminal toSerialize,
        // then it cannot be serialized, so we can ignore it
        for (int j = 0; j < component->bitOutputs.size(); j++)
        {
            BitOutputTerminal * output = component->bitOutputs[j];
            for (int k=0; k < output->connections.size(); k++)
            {
                BitConnection * connection = output->connections[k];
                // if the connection's InputTerminal does not belong to a component toSerialize,
                // then it cannot be serialized and will be ignored
                if (toSerialize.contains(connection->inputTerminal->component))
                    connections << connection;
            }
        }

        // byte connections
        for (int j = 0; j < component->byteOutputs.size(); j++)
        {
            ByteOutputTerminal * output = component->byteOutputs[j];
            for (int k = 0; k < output->connections.size(); k++)
            {
                ByteConnection * connection = output->connections[k];

                if (toSerialize.contains(connection->inputTerminal->component))
                    connections << connection;
            }
        }
    }

    // now save the connections
    for (int i=0; i < connections.size(); i++)
    {
        Connection * connection = connections[i];
        stream << "createConnection(";
        stream << connection->getOutputTerminal()->component->componentID << ","
               << connection->getInputTerminal()->component->componentID << ","
               << "\"" << connection->getOutputTerminal()->name << "\","
               << "\"" << connection->getInputTerminal()->name << "\","
               << connection->connectionType << ",";

        // serialize nodes
        stream << "{";
        for (int j = 0; j < connection->nodes.size(); j++)
        {
            if (j > 0) stream << ",";
            ConnectionNode * node = connection->nodes[j];
            // using stupid characters to delimit stuff again
            stream << node->pos().x() << "," << node->pos().y();
        }
        stream << "}";

        stream << ")\n";
    }

    return stream.readAll();
}


void MainWindow::createComponent(int type, int posX, int posY, int ID, int parentID)
{
    deserializeComponent(type, posX, posY, ID, parentID);
}

void MainWindow::changeComponentSettings(int ID, const char * settings)
{
    Component * component = myscene->findComponentWithOldId(ID);
    if (!component)
    {
        qDebug() << "Error changeComponentSettings(): component not found";
    }
    component->DeserializeSettings(settings);
}

void MainWindow::createConnection(const char * outputName, const int inputId, const int outputId, const char * inputName, QList<int> nodes, const int type)
{
    deserializeConnection(outputName, inputId, outputId, inputName, nodes, type);
}

Component * MainWindow::deserializeComponent(int type, int posX, int posY, int ID, int parentID)
{
    if (offsetPos && parentID == -1)
    {
        // looks nicer when pasting
        posX += 10;
        posY += 10;
    }

    Component * component = myscene->deserializeComponent(static_cast<ComponentType>(type), QPointF(posX, posY));
    component->oldID = ID; // if it worked properly we wouldn't need to do this
    component->setSelected(true);

    // search for and set parent if exists
    if (parentID != -1)
    {
        Component* parent = myscene->findComponentWithOldId(parentID);
        if (parent)
        {
            // set the parent
            // create move command to make it undoable
            myscene->undoStack->push(new MoveCommand(component,parent,0,component->pos(),component->pos()));
            // otherwise has graphics scene as parent, which is default anyway
        }
        else
        {
            qDebug() << "Error createComponent: parent component does not exist";
        }
    }

    return component;
}

void MainWindow::deserializeConnection(QString outputName, int inputId, int outputId, QString inputName, QList<int> nodes, int type)
{
    Component * outputComponent = myscene->findComponentWithOldId(outputId);
    Q_ASSERT(outputComponent);
    Component * inputComponent = myscene->findComponentWithOldId(inputId);
    Q_ASSERT(inputComponent);

    // find the terminals with the right name and type
    Connection * newConnection;
    if (static_cast<ConnectionType>(type) == ConnectionType::Bit)
    {
        BitOutputTerminal * outputTerminal = outputComponent->findBitOutput(outputName);
        Q_ASSERT(outputTerminal);
        BitInputTerminal * inputTerminal = inputComponent->findBitInput(inputName);
        Q_ASSERT(inputTerminal);

        // create a connection on the output terminal
        newConnection = new BitConnection(inputTerminal, outputTerminal);
    }
    else
    {
        ByteOutputTerminal * outputTerminal = outputComponent->findByteOutput(outputName);
        Q_ASSERT(outputTerminal);
        ByteInputTerminal * inputTerminal = inputComponent->findByteInput(inputName);
        Q_ASSERT(inputTerminal);

        // create a connection on the output terminal
        newConnection = new ByteConnection(inputTerminal, outputTerminal);
    }

    for (int i = 0; i < nodes.size(); i+=2)
    {
        ConnectionNode * newNode = new ConnectionNode(newConnection);
        newNode->setParentItem(newConnection->parentItem()->parentItem()->parentItem());
        newNode->setPos(nodes[i],nodes[i+1]);
        newConnection->nodes << newNode;
    }

    myscene->undoStack->push(new CreateConnectionCommand(newConnection));
    newConnection->updatePosition();
}

bool MainWindow::deserializeComponents(QTextStream *in)
{
    // will deserialize a list of components into the current network    

    // deselect all items
    QList<QGraphicsItem*> selected = myscene->selectedItems();
    for (int i = 0; i < selected.size(); i++)
        selected[i]->setSelected(false);

    bool res = !luaL_dostring(L, in->readAll().toLatin1().data());

    // deserialization complete -- reset all the oldIDs
    for (int i = 0; i < myscene->components.size(); i++)
    {
        myscene->components[i]->oldID = -1;
    }

    lua_getglobal(L, "simulationFrequency");
    if (lua_isinteger(L, -1))
    {
        int freq = lua_tointeger(L, -1);
        if (freq != simulationFrequency)
        {
            simulationFrequency = freq;
            shouldSignalColoursUpdate();
        }
    }      

    return res;
}


void MainWindow::on_action_Open_triggered()
{
    // check if current save file already exists
    if (!myscene->undoStack->isClean())
    {
        // can't open multiple projects at once, so ask if want to close current project
        int result = QMessageBox::question(this, tr("Confirm"), tr("The document has been modified.\n"
                                                                    "Do you want to save your changes?"),
                                               QMessageBox::Save | QMessageBox::Discard
                                               | QMessageBox::Cancel,
                                               QMessageBox::Save);
        switch (result) {
          case QMessageBox::Save:
              // Save was clicked
              on_action_Save_triggered(); // it doesn't feel right calling these functions but whatever, it seems to work
              break;
          case QMessageBox::Discard:
              // Don't Save was clicked, continue without saving
              break;
          case QMessageBox::Cancel:
              // Cancel was clicked, abort
              statusBar()->showMessage("Open File Canceled");
              return;
          default:
              // should never be reached
              break;
        }
    }

    // execute open file dialogue
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(),
            "Netlist files (*.net)");

    if (!fileName.isEmpty()) {
        // set as current save file
        currentSaveFile = fileName;
        setWindowTitle("LogicSimulator " + currentSaveFile);

        // open file
        QFile file(currentSaveFile);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        // delete all graphics items
        for (int i=0; i<myscene->components.size(); i++)
            delete myscene->components[i];
        myscene->components.clear();

        // do this too
        nextFreeComponentId = 0;

        QTextStream stream(&file);
        offsetPos = false;
        deserializeComponents(&stream);

        file.close();

        // should change title of program to name of open document and * asterisk when there are unsaved changes


        // clear undo stack
        myscene->undoStack->clear();

        statusBar()->showMessage("Opened File " + fileName);
    }
}

void MainWindow::save()
{
    QFile file(currentSaveFile);
    if (!file.open(QIODevice::WriteOnly)) {
        // error message
    } else {
        QTextStream stream(&file);
        stream << serializeComponents(myscene->components);
        stream.flush();
        file.close();
        statusBar()->showMessage(currentSaveFile + " Saved");
    }
}

void MainWindow::on_action_Save_triggered()
{
    // check if a save file already exists
    if (currentSaveFile.isEmpty())
    {
        // if not execute save file dialogue
        currentSaveFile = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                tr("Netlist Files (*.net)"));

        // if result is still empty then give up
        if (currentSaveFile.isEmpty())
            return;
    }

    save();
}

void MainWindow::on_actionSave_As_triggered()
{
    // execute save file dialogue
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Netlist Files (*.net)"));

    // if result is still empty then give up
    if (fileName.isEmpty())
        return;

    // otherwise set as current save file
    currentSaveFile = fileName;

    save();
}

void MainWindow::on_action_Undo_triggered()
{
    if (myscene->grabbedWireState)
        myscene->CancelWire();
    if (myscene->undoStack->canUndo())
        statusBar()->showMessage("Undo " + myscene->undoStack->undoText() + " successful"); // probably
    myscene->undoStack->undo();
}

void MainWindow::on_action_Redo_triggered()
{
    if (myscene->grabbedWireState)
        myscene->CancelWire();
    if (myscene->undoStack->canRedo())
        statusBar()->showMessage("Redo " + myscene->undoStack->redoText() + " successful");
    myscene->undoStack->redo();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Logic Simulator"),
                       tr("TODO Write a help document or something."));
}

void MainWindow::on_actionDeselect_triggered()
{
    noneButton.setChecked(true);
    myscene->activeTool = MyGraphicsScene::Tools::None;
    myscene->deletePreviewComponent();
}

void MainWindow::on_actionDelete_triggered()
{
    if (myscene->grabbedWireState)
        myscene->CancelWire();

    // if a connection is in hover state, delete that
    if (Connection::highlighted)
    {
        myscene->undoStack->push(new DeleteConnectionCommand(Connection::highlighted));
        Connection::highlighted = 0;
    }
    else
    {
        // delete selected units
        QList<Component *> selectedItems = mywindow->getSelectedComponents();
        // need to select children of selected items too
        QMutableListIterator<Component*> iter(selectedItems);
        while (iter.hasNext())
        {
            Component * item = iter.next();
            if (anyParentsInList(item,selectedItems))
                // remove this item from the list, as it will be selected again by getChildrenRecursive()
                iter.remove();
            // otherwise just leave it
        }
        QList<Component*> descendants;
        for (int i = 0; i < selectedItems.size(); i++)
            descendants << getChildrenRecursive(selectedItems[i]);
        selectedItems << descendants;
        // make a parallel list containing all the selectedItem's parents... hate
        QList<Component*> parents;
        for (int i = 0; i < selectedItems.size(); i++)
            parents << selectedItems[i]->parentComponent;
        myscene->undoStack->beginMacro("delete components");
        for (int i=0; i < selectedItems.size(); i++)
            myscene->undoStack->push(new DeleteCommand(selectedItems[i],parents[i]));
        myscene->undoStack->endMacro();
    }
}

void MainWindow::undoIndexChanged(int index)
{
    if (myscene->undoStack->canUndo())
        ui->action_Undo->setText("&Undo " + myscene->undoStack->command(index-1)->text());
    else
        ui->action_Undo->setText("&Undo");

    if (myscene->undoStack->canRedo())
        ui->action_Redo->setText("&Redo " + myscene->undoStack->command(index)->text());
    else
        ui->action_Redo->setText("&Redo");
}

QList<Component *> MainWindow::getSelectedComponents()
{
    QList<Component *> selectedComponents;

    for (int i = 0; i < myscene->components.size(); i++)
    {
        Component * component = myscene->components[i];
        if (component->isSelected())
            selectedComponents << component;
    }

    return selectedComponents;
}

bool MainWindow::anyParentsInList(Component *_component, QList<Component *> _list)
{
    // recursive. returns true if _toSerialize contains any of _component's ancestors
    if (_component->parentComponent)
    {
        if (_list.contains(_component->parentComponent))
            return true;
        else
            return anyParentsInList(_component->parentComponent,_list);
    }
    return false;
}

void MainWindow::resetBlockPanels()
{
    for (int i = 0; i < blockPanels.size(); i++)
        blockPanels[i]->setDefaultBrush();
}

void MainWindow::on_actionCopy_triggered()
{
    // serializes the selected components and any connections between them
    // saves the text to the clipboard
    QClipboard *clipboard = QApplication::clipboard();

    QList<Component *> selectedComponents = getSelectedComponents();

    clipboard->setText(serializeComponents(selectedComponents));

    statusBar()->showMessage("Copied To Clipboard");
}

void MainWindow::on_actionPaste_triggered()
{
    // deserializes the clipboard, if valid, into the scene

    // deselect all items
    QList<QGraphicsItem*> selectedItems = myscene->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++)
        selectedItems[i]->setSelected(false);

    myscene->undoStack->beginMacro("paste");

    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    QTextStream stream(&originalText);
    offsetPos = true;
    bool res = deserializeComponents(&stream);

    myscene->undoStack->endMacro();

    if (res)
        statusBar()->showMessage("Paste Successful");
    else
        statusBar()->showMessage(lua_tostring(L, -1));
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    if (!index.isValid()) return;
    QString fileName = model->fileName(index); // if you click a folder it says "index out of range"...
    QString filepath = model->filePath(index);

    // "don't show again" option?
    int result = QMessageBox::question(this, tr("Confirm"), "Do you want to import " + fileName +
                                                                "\n into the current network?",
                                           QMessageBox::Ok | QMessageBox::Cancel);
    switch (result) {
      case QMessageBox::Ok:
          // continue
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked, abort
          return;
      default:
          // should never be reached
          break;
    }

    if (!filepath.isEmpty()) {
        // open file
        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        QTextStream stream(&file);

        myscene->undoStack->beginMacro("import net");
        offsetPos = false;
        deserializeComponents(&stream);
        myscene->undoStack->endMacro();

        file.close();
    }
}

void MainWindow::on_actionSave_Se_lection_triggered()
{
    // pretty much the same as copy except it opens a file dialogue and saves it
    // execute save file dialogue
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Netlist Files (*.net)"));

    // if result is still empty then give up
    if (fileName.isEmpty())
        return;

    QList<Component *> selectedComponents = getSelectedComponents();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        // error message
    } else {
        QTextStream stream(&file);
        stream << serializeComponents(selectedComponents);
        stream.flush();
        file.close();
        statusBar()->showMessage("Saved Selected Components To " + fileName);
    }
}

void MainWindow::on_SimulationFrequencySpinBox_editingFinished()
{
    simulationFrequency = ui->SimulationFrequencySpinBox->value();
    // also reset the timer
    timer->start(simulationFrequency);

    shouldSignalColoursUpdate();
}

void MainWindow::shouldSignalColoursUpdate()
{
    if (StepMode)
        // always update. why not
        visibleSignals = true;
    else
    {
        if (simulationFrequency <= 10)
            // updating so fast would deck your computer
            visibleSignals = false;
        else
        {
            if (visibleSignalsPreferred)
                visibleSignals = true;
            else
                visibleSignals = false;
        }
    }
}

void MainWindow::on_PauseButton_clicked()
{
    StepMode = !StepMode;
    // toggle icon
    // toggle stepmode
    if (StepMode)
    {
        // QTimer.stop() doesn't work properly so just disconnect
        QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(Reevaluate()));
        ui->SimulationFrequencySpinBox->setDisabled(true);
        ui->PauseButton->setIcon(QIcon(":/Resources/Start.png"));
        ui->StepButton->setDisabled(false);
        statusBar()->showMessage("Simulation Paused");
    }
    else
    {
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(Reevaluate()));
        ui->SimulationFrequencySpinBox->setDisabled(false);
        ui->PauseButton->setIcon(QIcon(":/Resources/Pause.png"));
        ui->StepButton->setDisabled(true);
        statusBar()->showMessage("Simulation Resumed");
    }

    shouldSignalColoursUpdate();
}

void MainWindow::on_StepButton_clicked()
{
    // reevaluate the state of the simulation once
    Reevaluate();
}

// -----------------------------------------------
//                  TOOL  BUTTONS
// -----------------------------------------------

void MainWindow::on_AdderToolButton_clicked()
{
    // Set paint state on state machine, and set component to paint as Adder.
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tAlu;
    myscene->createPreviewComponent(ComponentType::tAlu);
}

void MainWindow::on_AndGateToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tAndGate;
    myscene->createPreviewComponent(ComponentType::tAndGate);
}

void MainWindow::on_BlockToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tBlock;
    myscene->createPreviewComponent(ComponentType::tBlock);
}

void MainWindow::on_NotGateToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tNotGate;
    myscene->createPreviewComponent(ComponentType::tNotGate);
}

void MainWindow::on_OrGateToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tOrGate;
    myscene->createPreviewComponent(ComponentType::tOrGate);
}

void MainWindow::on_ClockToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tClock;
    myscene->createPreviewComponent(ComponentType::tClock);
}

void MainWindow::on_InputToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tInput;
    myscene->createPreviewComponent(ComponentType::tInput);
}

void MainWindow::on_OutputToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tOutput;
    myscene->createPreviewComponent(ComponentType::tOutput);
}

void MainWindow::on_MemoryToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tMemory;
    myscene->createPreviewComponent(ComponentType::tMemory);
}

void MainWindow::on_NorToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tNor;
    myscene->createPreviewComponent(ComponentType::tNor);
}

void MainWindow::on_NandToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tNand;
    myscene->createPreviewComponent(ComponentType::tNand);
}

void MainWindow::on_XorToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tXor;
    myscene->createPreviewComponent(ComponentType::tXor);
}

void MainWindow::on_CounterToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tCounter;
    myscene->createPreviewComponent(ComponentType::tCounter);
}

void MainWindow::on_FlipFlopToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tFlipFlop;
    myscene->createPreviewComponent(ComponentType::tFlipFlop);
}

void MainWindow::on_ControlToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tControl;
    myscene->createPreviewComponent(ComponentType::tControl);
}

void MainWindow::on_MuxToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tMux;
    myscene->createPreviewComponent(ComponentType::tMux);
}

void MainWindow::on_UpToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tUp;
    myscene->createPreviewComponent(ComponentType::tUp);
}

void MainWindow::on_DownToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tDown;
    myscene->createPreviewComponent(ComponentType::tDown);
}

void MainWindow::on_EqualityToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tEquality;
    myscene->createPreviewComponent(ComponentType::tEquality);
}

void MainWindow::on_RandomToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tRandom;
    myscene->createPreviewComponent(ComponentType::tRandom);
}

void MainWindow::on_DelayToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tDelay;
    myscene->createPreviewComponent(ComponentType::tDelay);
}

void MainWindow::on_DemuxToolButton_clicked()
{
    myscene->activeTool = MyGraphicsScene::Tools::MakeComponent;
    myscene->activeComponent = ComponentType::tDemux;
    myscene->createPreviewComponent(ComponentType::tDemux);

}


void MainWindow::on_actionZoom_In_triggered()
{
    ui->graphicsView->scale(1.1,1.1);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    ui->graphicsView->scale(0.9,0.9);
}

void MainWindow::on_actionUpdate_Wire_Colours_triggered()
{
    visibleSignalsPreferred = !visibleSignalsPreferred;

    shouldSignalColoursUpdate();
}
