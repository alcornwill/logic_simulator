#include "mygraphicsscene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include "outputterminal.h"
#include "connection.h"
#include "inputterminal.h"
#include <QList>
#include <QUndoStack>
#include "createcommand.h"
#include "createconnectioncommand.h"
#include "deleteconnectioncommand.h"
#include "mainwindow.h"
#include "orgate.h"
#include "andgate.h"
#include "blockcomponent.h"
#include "notgate.h"
#include "andgate.h"
#include "blocknodecomponent.h"
#include "blockpanel.h"
#include "bitconnection.h"
#include "byteconnection.h"
#include "bitinputterminal.h"
#include "byteinputterminal.h"
#include "bitoutputterminal.h"
#include "byteoutputterminal.h"
#include "inputcomponent.h"
#include "outputcomponent.h"
#include "memorycomponent.h"
#include "muxcomponent.h"
#include "flipflopcomponent.h"
#include "upcomponent.h"
#include "downcomponent.h"
#include "countercomponent.h"
#include "xorcomponent.h"
#include "nandcomponent.h"
#include "norcomponent.h"
#include "controlcomponent.h"
#include "randomcomponent.h"
#include "equalitycomponent.h"
#include "clockcomponent.h"
#include "alucomponent.h"
#include <QGraphicsSceneWheelEvent>
#include "ui_mainwindow.h"
#include <QGraphicsView>
#include "delaycomponent.h"
#include "demux.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    undoStack = new QUndoStack(this);

    setSceneRect(-1000,-1000,2000,2000); // make it quite big
    // unfortunately this still lets the user move components outside the scene, but there's always Undo so whatever
}

void MyGraphicsScene::CancelWire()
{
    // cancel wire
    delete wire;
    grabbedWireState = false;
}

Component *MyGraphicsScene::findComponentWithId(int id)
{
    for (int i=0; i<components.size(); i++)
    {
        Component * component = components[i];
        if (component->componentID == id)
            return component;
    }
    return 0;
}

Component *MyGraphicsScene::findComponentWithOldId(int id)
{
    for (int i=0; i<components.size(); i++)
    {
        Component * component = components[i];
        if (component->oldID == id)
            return component;
    }
    return 0;
}

void MyGraphicsScene::createPreviewComponent(ComponentType type, QPointF mousePoint)
{
    // if there already was one delete it
    if (previewComponent)
        deletePreviewComponent();

    previewComponent = createComponent(type);
    previewComponent->setOpacity(0.3);
    previewComponent->setZValue(2); // above panels (above everything)
    addItem(previewComponent);
    previewComponent->setPos(mousePoint);
}

Component *MyGraphicsScene::createComponent(ComponentType type)
{
    switch (type)
    {
    case ComponentType::tOrGate:
        return new OrGate();
    case ComponentType::tBlock:
        return new BlockComponent();
    case ComponentType::tNotGate:
        return new NotGate();
    case ComponentType::tAndGate:
        return new AndGate();
    case ComponentType::tInput:
        return new InputComponent();
    case ComponentType::tOutput:
        return new OutputComponent();
    case ComponentType::tMemory:
        return new MemoryComponent();
    case ComponentType::tXor:
        return new XorComponent();
    case ComponentType::tNor:
        return new NorComponent();
    case ComponentType::tNand:
        return new NandComponent();
    case ComponentType::tMux:
        return new MuxComponent();
    case ComponentType::tFlipFlop:
        return new FlipFlopComponent();
    case ComponentType::tCounter:
        return new CounterComponent();
    case ComponentType::tUp:
        return new UpComponent();
    case ComponentType::tDown:
        return new DownComponent();
    case ComponentType::tControl:
        return new ControlComponent();
    case ComponentType::tRandom:
        return new RandomComponent();
    case ComponentType::tEquality:
        return new EqualityComponent();
    case ComponentType::tClock:
        return new ClockComponent();
    case ComponentType::tAlu:
        return new AluComponent();
    case ComponentType::tDelay:
        return new DelayComponent();
    case ComponentType::tDemux:
        return new Demux();
    default:
        return 0;
    }
}

void MyGraphicsScene::deletePreviewComponent()
{
    delete previewComponent;
    previewComponent = 0;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::RightButton)
    {
        if (grabbedWireState)
        {
            CancelWire();
            return; // don't let the event propogate
        }
        if (activeTool != Tools::None)
        {
            mywindow->resetBlockPanels();
            mywindow->on_actionDeselect_triggered();
            return;
        }
    }

    if (mouseEvent->button() == Qt::LeftButton)
    {
        // this method does lots of different things depending on the active tool
        switch (activeTool)
        {
            case Tools::MakeComponent:
                placeComponent(activeComponent, mouseEvent->scenePos());
                break;
            default:
                break;
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (grabbedWireState)
        wire->setLine(QLineF(wireStart,mouseEvent->scenePos()));
    else if (activeTool == Tools::MakeComponent)
    {
        // update preview component position
        previewComponent->setPos(mouseEvent->scenePos());
        // and do this
        previewComponent->findColliding();
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyGraphicsScene::initializeComponent(Component *newComponent, QPointF mousePoint, ComponentType type, Component*parent)
{
    newComponent->type = type;
    newComponent->setFlag(QGraphicsItem::ItemIsSelectable);
    newComponent->setFlag(QGraphicsItem::ItemIsMovable);
    newComponent->setPos(mousePoint);
    newComponent->Initialize();

    undoStack->push(new CreateCommand(newComponent,parent));
}

Component* MyGraphicsScene::placeComponent(ComponentType type, QPointF mousePoint)
{
    // this method is called by the user

    Q_ASSERT(previewComponent); // don't reall see how this could happen

    Component *newComponent = previewComponent;
    previewComponent = 0; // don't let my stupid code delete the nice new component
    newComponent->setZValue(0);
    createPreviewComponent(type, mousePoint); // create another preview
    newComponent->setOpacity(1.0);


    Component * parent = newComponent->findColliding(); // if returns 0 then still fine
    // this is horrible
    if (parent)
    {
        BlockComponent * blockComponent = static_cast<BlockComponent*>(parent);
        mousePoint -= blockComponent->blockPanel->scenePos();
    }

    removeItem(newComponent); // annoying
    // megahax - call this again to correct the colour of the panel
    // since the item was removed from the scene, the colour will go back to default
    newComponent->findColliding();

    initializeComponent(newComponent, mousePoint, type, parent);

    return newComponent;
}

Component *MyGraphicsScene::deserializeComponent(ComponentType type, QPointF position)
{
    // this is just like createComponent except it doesn't create the transparent version first
    Component * newComponent = createComponent(type);
    initializeComponent(newComponent, position, type, 0); // set parent to 0 - we change it later
    return newComponent;
}

void MyGraphicsScene::createByteWire(ByteOutputTerminal *_byteOutputTerminal)
{
    if (!createWire()) return;

    wireType = ConnectionType::Byte;
    byteOutputTerminal = _byteOutputTerminal;
    wireStart = byteOutputTerminal->scenePos();
}

void MyGraphicsScene::createBitWire(BitOutputTerminal *_bitOutputTerminal)
{
    if (!createWire()) return;

    wireType = ConnectionType::Bit;
    bitOutputTerminal = _bitOutputTerminal;
    wireStart = bitOutputTerminal->scenePos();    
}

bool MyGraphicsScene::createWire()
{
    if (activeTool == Tools::None && !grabbedWireState)
    {
        // create wire
        grabbedWireState = true;
        wire = addLine(QLineF(-1000,-1000,-1000,-1000));
        QPen peni = wire->pen();
        peni.setWidth(2);
        wire->setPen(peni);
        return true;
    }
    return false;
}


void MyGraphicsScene::createBitConnection(BitInputTerminal *_bitInputTerminal)
{
    // if connection is valid, make the connection

    if (wireType != ConnectionType::Bit) return;
    if (!createConnection(_bitInputTerminal, bitOutputTerminal)) return;

    BitConnection * newConnection = new BitConnection(_bitInputTerminal, bitOutputTerminal);

    // if input terminal already has a connection
    if (_bitInputTerminal->connected)
    {
        // check if input and output terminals are already connected
        if (_bitInputTerminal->outputTerminal == bitOutputTerminal) return;

        // otherwise, delete the other old connection first
        undoStack->beginMacro("replace connection");
        undoStack->push(new DeleteConnectionCommand(_bitInputTerminal->connection));
        undoStack->push(new CreateConnectionCommand(newConnection));
        undoStack->endMacro();
    }
    else
        undoStack->push(new CreateConnectionCommand(newConnection));

    newConnection->updatePosition();
}

void MyGraphicsScene::createByteConnection(ByteInputTerminal *_byteInputTerminal)
{
    // duplicate code of createBitConnection
    if (wireType != ConnectionType::Byte) return;
    if (!createConnection(_byteInputTerminal, byteOutputTerminal)) return;

    ByteConnection * newConnection = new ByteConnection(_byteInputTerminal, byteOutputTerminal);

    // if input terminal already has a connection
    if (_byteInputTerminal->connected)
    {
        // check if input and output terminals are already connected
        if (_byteInputTerminal->outputTerminal == byteOutputTerminal) return;

        // otherwise, delete the other old connection first
        undoStack->beginMacro("replace connection");
        undoStack->push(new DeleteConnectionCommand(_byteInputTerminal->connection));
        undoStack->push(new CreateConnectionCommand(newConnection));
        undoStack->endMacro();
    }
    else
        undoStack->push(new CreateConnectionCommand(newConnection));

    newConnection->updatePosition();
}

bool MyGraphicsScene::createConnection(InputTerminal* _inputTerminal, OutputTerminal* _outputTerminal)
{
    if (!grabbedWireState) return false;

    // if terminals do not have the same parent they are not allowed to be connected
    Component * inputParent = _inputTerminal->getParent();
    Component * outputParent = _outputTerminal->getParent();
    if (inputParent && outputParent)
    {
        if (inputParent->componentID != outputParent->componentID) return false;
    }
    else if (inputParent || outputParent) return false;

    // otherwise looks ok
    return true;
}


