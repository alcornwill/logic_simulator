#include "component.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include "mygraphicsscene.h"
#include <QDebug>
#include "connection.h"
#include <QUndoStack>
#include "movecommand.h"
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QToolButton>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHashIterator>
#include "blockpanel.h"
#include "deleteconnectioncommand.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"
#include "bitconnection.h"
#include "byteconnection.h"
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>

BitInputTerminal *Component::addBitInputTerminal(QString name)
{
    BitInputTerminal *newterminal = new BitInputTerminal();
    newterminal->name = name;
    newterminal->setToolTip(name);
    newterminal->setParentItem(this);
    newterminal->component = this;
    bitInputs << newterminal; // I may need a parameter for insert position
    resize();
    return newterminal;
}

BitOutputTerminal *Component::addBitOutputTerminal(QString name)
{
    BitOutputTerminal *newterminal = new BitOutputTerminal();
    newterminal->name = name;
    newterminal->setToolTip(name);
    newterminal->setParentItem(this);
    newterminal->component = this;
    bitOutputs << newterminal;
    resize();
    return newterminal;
}

ByteInputTerminal *Component::addByteInputTerminal(QString name)
{
    ByteInputTerminal *newterminal = new ByteInputTerminal();
    newterminal->name = name;
    newterminal->setToolTip(name);
    newterminal->setParentItem(this);
    newterminal->component = this;
    byteInputs << newterminal; // I may need a parameter for insert position
    resize();
    return newterminal;
}

ByteOutputTerminal *Component::addByteOutputTerminal(QString name)
{
    ByteOutputTerminal *newterminal = new ByteOutputTerminal();
    newterminal->name = name;
    newterminal->setToolTip(name);
    newterminal->setParentItem(this);
    newterminal->component = this;
    byteOutputs << newterminal;
    resize();
    return newterminal;
}

void Component::removeBitInputTerminal()
{
    BitInputTerminal * terminal = bitInputs.takeLast(); // remove from inputs
    resize();

    // this should delete the connection too, and disconnect it first
    delete terminal;
}

void Component::removeBitOutputTerminal()
{
    BitOutputTerminal * terminal = bitOutputs.takeLast();
    resize();
    delete terminal;
}

void Component::removeByteInputTerminal()
{
    ByteInputTerminal * terminal = byteInputs.takeLast();
    resize();
    delete terminal;
}

void Component::removeByteOutputTerminal()
{
    ByteOutputTerminal * terminal = byteOutputs.takeLast();
    resize();
    delete terminal;
}

void Component::resize()
{
    // use the number of inputs and outputs to determine dimensions
    // get number inputs/outputs
    prepareGeometryChange();

    QList<InputTerminal*> inputs = getInputTerminals();
    QList<OutputTerminal*> outputs = getOutputTerminals();

    int numberInputs = inputs.size();
    int numberOutputs = outputs.size();

    // all components are same width? (40 units)

    int spacing = 10; // distance above/below terminal

    // first, define the size of the component by finding the greater of inputs and outputs
    int max = qMax(numberInputs, numberOutputs);

    if (!fixedHeight)
        height = qMax(2*spacing*max, 40); // 40 is minimum


    rect.setRect(-width/2,-height/2,width,height);

    int y=(numberInputs-1) * -spacing;

    // now reposition the terminals
    for (int i = 0; i < inputs.size(); i++)
    {
        InputTerminal *input = inputs[i];
        input->setPos(QPointF(-5-width/2,y));
        y+=spacing*2;

        input->updatePosition();
    }

    y=(numberOutputs-1) * -spacing;

    for (int j = 0; j < outputs.size(); j++)
    {
        OutputTerminal *output = outputs[j];
        output->setPos(QPointF(5+width/2,y));
        y+=spacing*2;

        output->updatePositions();
    }

    update();
}

QList<InputTerminal *> Component::getInputTerminals()
{
    // always returns bit terminals before byte terminals
    QList<InputTerminal *> inputs;

    for (int i = 0; i < bitInputs.size(); i++)
        inputs << bitInputs[i];

    for (int i = 0; i < byteInputs.size(); i++)
        inputs << byteInputs[i];

    return inputs;
}

QList<OutputTerminal *> Component::getOutputTerminals()
{
    QList<OutputTerminal *> outputs;

    for (int i = 0; i < bitOutputs.size(); i++)
        outputs << bitOutputs[i];

    for (int i = 0; i < byteOutputs.size(); i++)
        outputs << byteOutputs[i];

    return outputs;
}

Component::Component()
{
    pen.setColor(Qt::black);
    pen.setWidth(4);
    pen.setJoinStyle(Qt::MiterJoin);
    resize();
    setData(0,QVariant("component")); // i've only been using index 0 so far. and the tags are magic numbers
}

void Component::Initialize()
{
    componentID = mywindow->newComponentId();

    setFlag(ItemSendsGeometryChanges);
}

void Component::setChildComponent(Component *component)
{
    // by default, components cannot have child components
    // I think only Block component will implement this
    qDebug() << "tried to set as child of incompatible component";
    return;
}

void Component::removeChildComponent(Component *component)
{
    // ditto
}

void Component::Reevaluate()
{
    if (reevaluated) return;
    reevaluated = true;

    // ensure inputs are fresh
    for (int i = 0; i < bitInputs.size(); i++)
    {
        BitInputTerminal * input = bitInputs[i];
        if (input->connected) input->outputTerminal->component->Reevaluate();
    }

    for (int i = 0; i < byteInputs.size(); i++)
    {
        ByteInputTerminal * input = byteInputs[i];
        if (input->connected) input->outputTerminal->component->Reevaluate();
    }

    // do reevaluation logic
    ReevaluateLogic();

    // tell output terminals to update wire colours
    if (mywindow->visibleSignals)
    {
        for (int j = 0; j < bitOutputs.size(); j++)
            bitOutputs[j]->updateWireColors();

        for (int j = 0; j < byteOutputs.size(); j++)
            byteOutputs[j]->updateWireColors();
    }
}

void Component::saveStartPos()
{
    startMove = pos();
    startParent = parentComponent;
}

MoveCommand *Component::getMoveCommand()
{
    // this is used to add multiple component move commands onto the stack at the same time, as a group
    endMove = pos();
    if (endParent != startParent)
        // then endMove will not be correct
        if (endParent)
            endMove = scenePos() - endPanel->scenePos();
        else
            // scene is the parent (maybe)
            endMove = scenePos();

    return new MoveCommand(this, endParent, startParent, startMove, endMove);
}

BitInputTerminal *Component::findBitInput(QString name)
{
    for (int i = 0; i < bitInputs.size(); i++)
        if (bitInputs[i]->name == name)
            return bitInputs[i];

    return 0;
}

BitOutputTerminal *Component::findBitOutput(QString name)
{
    for (int i = 0; i < bitOutputs.size(); i++)
        if (bitOutputs[i]->name == name)
            return bitOutputs[i];

    return 0;
}

ByteInputTerminal *Component::findByteInput(QString name)
{
    for (int i = 0; i < byteInputs.size(); i++)
        if (byteInputs[i]->name == name)
            return byteInputs[i];

    return 0;
}

ByteOutputTerminal *Component::findByteOutput(QString name)
{
    for (int i = 0; i < byteOutputs.size(); i++)
        if (byteOutputs[i]->name == name)
            return byteOutputs[i];

    return 0;
}

void Component::deleteAllConnections()
{
    for (int i = 0; i < bitInputs.size(); i++)
    {
        BitInputTerminal * input = bitInputs[i];
        if (input->connected)
            myscene->undoStack->push(new DeleteConnectionCommand(input->connection));
    }

    for (int i = 0; i < bitOutputs.size(); i++)
    {
        BitOutputTerminal *out = bitOutputs[i];
        QMutableListIterator<BitConnection*> iter(out->connections);
        while (iter.hasNext())
            myscene->undoStack->push(new DeleteConnectionCommand(iter.next()));
    }

    for (int i = 0; i < byteInputs.size(); i++)
    {
        ByteInputTerminal * input = byteInputs[i];
        if (input->connected)
            myscene->undoStack->push(new DeleteConnectionCommand(input->connection));
    }

    for (int i = 0; i < byteOutputs.size(); i++)
    {
        ByteOutputTerminal *out = byteOutputs[i];
        QMutableListIterator<ByteConnection*> iter(out->connections);
        while (iter.hasNext())
            myscene->undoStack->push(new DeleteConnectionCommand(iter.next()));
    }
}

QToolButton * Component::CreateButtonSetting(QString name, const QObject *receiver, const char *method)
{
    // automatically creates a button with specified name, with signal and slot connection, and shoves it into PropertiesDock
    QToolButton *button = new QToolButton();
    button->setText(name);
    QObject::connect(button, SIGNAL(pressed()), receiver, method);
    mywindow->ui->verticalLayout_4->addWidget(button);
    return button;
}

QComboBox *Component::CreateComboBoxSetting(const QObject *receiver, const char *method)
{
    QComboBox *comboBox = new QComboBox();
    QObject::connect(comboBox, SIGNAL(activated(int)), receiver, method);
    mywindow->ui->verticalLayout_4->addWidget(comboBox);
    return comboBox; // add the items yourself
}

QCheckBox *Component::CreateCheckBoxSetting(QString text, bool value, const QObject *receiver, const char *method)
{
    QCheckBox *checkBox = new QCheckBox();
    checkBox->setText(text);
    checkBox->setChecked(value);
    QObject::connect(checkBox, SIGNAL(stateChanged(int)), receiver, method);
    mywindow->ui->verticalLayout_4->addWidget(checkBox);
    return checkBox;
}

QSpinBox *Component::CreateIntSetting(QString text, int number, const QObject *receiver, const char *method)
{
    QSpinBox *spinBox = new QSpinBox();
    spinBox->setToolTip(text);
    spinBox->setMaximum(65535);
    spinBox->setMinimum(0);
    spinBox->setValue(number);
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), receiver, method);
    mywindow->ui->verticalLayout_4->addWidget(spinBox);
    return spinBox;
}

QLineEdit *Component::CreateStringSetting(QString text, const QObject *receiver, const char *method)
{
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setText(text);
    QObject::connect(lineEdit, SIGNAL(textEdited(QString)), receiver, method);
    mywindow->ui->verticalLayout_4->addWidget(lineEdit);
    return lineEdit;
}

QRectF Component::boundingRect() const
{
    qreal feather = 4; // the pen width...
    return QRectF(-(width+feather)/2,-(height+feather)/2,width+feather,height+feather);
}

void Component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // default appearance
    if (option->state & QStyle::State_Selected)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::black);

    painter->setPen(pen);
    painter->drawRect(rect);
    painter->fillRect(rect,QBrush(QColor(Qt::white)));
    painter->drawPixmap(-16,-16,32,32,*image); // assuming image will be 16x16
}

void Component::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (myscene->activeTool == MyGraphicsScene::Tools::None &&
        event->button() == Qt::LeftButton)
    {
        hasMoved = false;
        if (!isSelected()) saveStartPos();
        // for all selected components, make them remember their start position
        QList<Component*> selectedComponents = mywindow->getSelectedComponents();
        for (int i=0; i<selectedComponents.size(); i++)
                selectedComponents[i]->saveStartPos();
    }

    QGraphicsItem::mousePressEvent(event);
}

void Component::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // if item was being dragged
    if (myscene->activeTool == MyGraphicsScene::Tools::None &&
        event->button() == Qt::LeftButton &&
        hasMoved)
    {     
        QList<Component*> selectedItems = mywindow->getSelectedComponents();
        if (selectedItems.size() > 1)
        {
            myscene->undoStack->beginMacro("move components");
            // woah, could use event->buttonDownPos instead of saving original position! maybe
            for (int i=0; i<selectedItems.size(); i++)
                myscene->undoStack->push(selectedItems[i]->getMoveCommand());
        }
        else
        {
            myscene->undoStack->beginMacro("move component"); // only difference is name of macro...
            myscene->undoStack->push(getMoveCommand()); // assuming one selected component must be this
        }

        if (endParent != startParent)
            // delete all connections
            // there would be more usable solutions to this but they would be difficult to implement
            // maybe you could have a MoveComponents command?
            // it would be nice at least if we highlighted the connections red, to communicate that they're going to be deleted
            deleteAllConnections();

        myscene->undoStack->endMacro();

        // also need to make all block panels return to default brush
        mywindow->resetBlockPanels();

        hasMoved = false;
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

Component * Component::findColliding()
{
    QList<QGraphicsItem*> colliding = collidingItems(Qt::ContainsItemShape);

    for (int i = 0; i < colliding.size(); i++)
    {
        QGraphicsItem *item = colliding[i];
        if (item->data(0).toString() == "blockpanel")
        {
            BlockPanel * panel = static_cast<BlockPanel*>(item);
            if (panel)
            {
                endParent = panel->blockComponent;
                if (endPanel && endPanel != panel)
                    endPanel->setDefaultBrush();
                endPanel = panel;
                if (parentItem() != endPanel)
                    endPanel->setItemDropBrush();
                return endParent; // stop looking - should only have found topmost panel
            }
            else
                qDebug() << "could not cast block panel";
        }
    }
    endParent = 0;
    if (endPanel)
        endPanel->setDefaultBrush();
    endPanel = 0;
    return 0;
}

void Component::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(mywindow->ui->actionSave_Se_lection);
    menu.exec(event->screenPos());
}

QVariant Component::itemChange(GraphicsItemChange change, const QVariant & value)
{
    // tell all underlings to update wire positions
    if (change == ItemPositionHasChanged)
    {
        hasMoved = true;
        for (int i = 0; i < bitOutputs.size(); i++)
            bitOutputs[i]->updatePositions();

        for (int j = 0; j < bitInputs.size(); j++)
            bitInputs[j]->updatePosition();

        for (int i = 0; i < byteOutputs.size(); i++)
            byteOutputs[i]->updatePositions();

        for (int j = 0; j < byteInputs.size(); j++)
            byteInputs[j]->updatePosition();

        if (myscene->mouseGrabberItem()) // if an item is being moved (we must be one of them)
            // find out if we should still have the same parent
            findColliding();
    }

    if (change == ItemSelectedHasChanged)
    {
        // wipe settings
        mywindow->deleteAll(mywindow->ui->verticalLayout_4);

        if (value.toBool())
            createSettings();
    }

    return QGraphicsItem::itemChange(change, value);
}
