#include "blockcomponent.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include "bitblocknodecomponent.h"
#include "byteblocknodecomponent.h"
#include "outputterminal.h"
#include "inputterminal.h"
#include "connection.h"
#include "mygraphicsscene.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include "blockpanel.h"
#include "mainwindow.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"
#include "myresources.h"
#include <QStyleOptionGraphicsItem>
#include <QStyle>

BlockComponent::BlockComponent()
{
    // create the block panel
    blockPanel = new BlockPanel(0,0,200,200,this);
    blockPanel->setPos(-100,-200);
    blockPanel->setFlag(QGraphicsItem::ItemIsMovable);
    blockPanel->blockComponent = this;
    blockPanel->setZValue(1);
    blockPanel->setVisible(false);
    blockPanel->setDefaultBrush();

    image = &myresources->block;
}

BlockComponent::~BlockComponent()
{
    // kill nodes
    // (not bothering to clear the qlists because we're dying anyway)
    for (int i = 0; i < bitInputNodes.size(); i++)
        // wonder if this will break anything
        delete bitInputNodes[i];

    for (int i = 0; i < bitOutputNodes.size(); i++)
        delete bitOutputNodes[i];

    for (int i = 0; i < byteInputNodes.size(); i++)
        delete byteInputNodes[i];

    for (int i = 0; i < byteOutputNodes.size(); i++)
        delete byteOutputNodes[i];
}

void BlockComponent::ReevaluateLogic()
{
    // do nothing
}

void BlockComponent::setChildComponent(Component *component)
{
    // if child already had parent, remove that first
    if (component->parentComponent)
        component->parentComponent->removeChildComponent(component);

    component->parentComponent = this;
    component->setParentItem(blockPanel);
    // add to child components list
    childComponents << component;
    // it's going to be difficult to make sure childComponents never contains null pointers and stuff
}

void BlockComponent::removeChildComponent(Component *component)
{
    bool result = childComponents.removeOne(component);
    Q_ASSERT(result);
    if (!result) return;
    component->parentComponent = 0;
}

void BlockComponent::resize()
{
    // repositions the terminals of the block nodes

    // do the standard resizing with the outside terminals
    Component::resize();

    // do special resizing for ones on the blockPanel
    // pretty much the same thing as resize except it doesn't scale the block panel (maybe it should...)

    // get all nodes (bit and byte)
    QList<InputTerminal*> blockInputs;
    blockInputs << blockBitInputs; // nice. we can do this because bitInputNodes actually holds base class pointers
    blockInputs << blockByteInputs;

    QList<OutputTerminal*> blockOutputs;
    blockOutputs << blockBitOutputs;
    blockOutputs << blockByteOutputs;

    int numberInputs = blockInputs.size();
    int numberOutputs = blockOutputs.size();

    // all components are same width? (40 units)

    int spacing = 10; // distance above/below terminal

    int y=((numberInputs-1) * -spacing) + (blockPanel->rect().height()/2);

    // now reposition the terminals
    for (int i = 0; i < blockInputs.size(); i++)
    {
        InputTerminal *input = blockInputs[i];
        input->setPos(QPointF(-5+blockPanel->rect().width(),y));
        y+=spacing*2;

        input->updatePosition();
    }

    y=((numberOutputs-1) * -spacing) + (blockPanel->rect().height()/2);

    for (int j = 0; j < blockOutputs.size(); j++)
    {
        OutputTerminal *output = blockOutputs[j];
        output->setPos(QPointF(5,y));
        y+=spacing*2;

        output->updatePositions();
    }

    myscene->update();
}

void BlockComponent::Initialize()
{
    initialized = true;

    // also initialize block nodes
    for (int i = 0; i < bitInputNodes.size(); i++)
        bitInputNodes[i]->Initialize();

    for (int i = 0; i < bitOutputNodes.size(); i++)
        bitOutputNodes[i]->Initialize();

    for (int i = 0; i < byteInputNodes.size(); i++)
        byteInputNodes[i]->Initialize();

    for (int i = 0; i < byteOutputNodes.size(); i++)
        byteOutputNodes[i]->Initialize();


    Component::Initialize();
}

void BlockComponent::createSettings()
{
    CreateStringSetting(name, this, SLOT(nameChanged(QString)));

    CreateButtonSetting("Add Bit Input Terminal", this, SLOT(addBlockBitInput()));
    CreateButtonSetting("Remove Bit Input Terminal", this, SLOT(removeBlockBitInput()));
    CreateButtonSetting("Add Bit Output Terminal", this, SLOT(addBlockBitOutput()));
    CreateButtonSetting("Remove Bit Output Terminal", this, SLOT(removeBlockBitOutput()));

    CreateButtonSetting("Add Byte Input Terminal", this, SLOT(addBlockByteInput()));
    CreateButtonSetting("Remove Byte Input Terminal", this, SLOT(removeBlockByteInput()));
    CreateButtonSetting("Add Byte Output Terminal", this, SLOT(addBlockByteOutput()));
    CreateButtonSetting("Remove Byte Output Terminal", this, SLOT(removeBlockByteOutput()));
}

QString BlockComponent::SerializeSettings()
{
    // serializing the block panel position and dimensions, and the number of terminals

    QString string = name + ",";

    QRectF getRect = blockPanel->rect();
    string += QString::number(blockPanel->pos().x()) + "," +
                     QString::number(blockPanel->pos().y()) + "," +
                     QString::number(getRect.topLeft().x()) + "," +
                     QString::number(getRect.topLeft().y()) + "," +
                     QString::number(getRect.width()) + "," +
                     QString::number(getRect.height());

    // mega hax. wish I'd used json
    string += ".";

    for (int i = 0; i < bitInputNodes.size(); i++)
        string += QString::number(bitInputNodes[i]->componentID) + ","; // this will create an extra , at the end but QString::SkipEmptyParts handles it

    string += ".";

    for (int i = 0; i < bitOutputNodes.size(); i++)
        string += QString::number(bitOutputNodes[i]->componentID) + ",";

    string += ".";

    for (int i = 0; i < byteInputNodes.size(); i++)
        string += QString::number(byteInputNodes[i]->componentID) + ",";

    string += ".";

    for (int i = 0; i < byteOutputNodes.size(); i++)
        string += QString::number(byteOutputNodes[i]->componentID) + ",";

    return  string;
}

void BlockComponent::DeserializeSettings(QString string)
{
    QStringList split = string.split('.');

    QStringList split0 = split[0].split(',');
    name = split0[0];
    blockPanel->setPos(split0[1].toInt(),split0[2].toInt());
    blockPanel->setRect(QRect(split0[3].toInt(),
                              split0[4].toInt(),
                              split0[5].toInt(),
                              split0[6].toInt()));

    QStringList split1 = split[1].split(',',QString::SkipEmptyParts);
    for (int i = 0; i < split1.size(); i++)
    {
        Component * node = addBlockBitInput();
        node->oldID = split1[i].toInt();
    }

    QStringList split2 = split[2].split(',',QString::SkipEmptyParts);
    for (int i = 0; i < split2.size(); i++)
    {
        Component * node = addBlockBitOutput();
        node->oldID = split2[i].toInt();
    }

    QStringList split3 = split[3].split(',',QString::SkipEmptyParts);
    for (int i = 0; i < split3.size(); i++)
    {
        Component * node = addBlockByteInput();
        node->oldID = split3[i].toInt();
    }

    QStringList split4 = split[4].split(',',QString::SkipEmptyParts);
    for (int i = 0; i < split4.size(); i++)
    {
        Component * node = addBlockByteOutput();
        node->oldID = split4[i].toInt();
    }
}

void BlockComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    blockPanel->setVisible(!blockPanel->isVisible());

    QGraphicsObject::mousePressEvent(event);
}

Component * BlockComponent::addBlockBitInput()
{
    int numberInputNodes = bitInputNodes.size();
    if (numberInputNodes >= 20) return 0;

    BitBlockNodeComponent * node = new BitBlockNodeComponent(numberInputNodes,true);
    node->createTerminals(); // can't call this in constructor because c++ is gay
    myscene->components << node;
    setChildComponent(node);
    bitInputNodes << node;

    // since this is an input node, the output terminal should be child of the block panel
    OutputTerminal * output = node->bitOutputs.first();
    output->setParentItem(blockPanel);
    blockBitOutputs << output;

    // and the input should be a child of this component
    BitInputTerminal * input = node->bitInputs.first();
    input->setParentItem(this);
    input->component = this; // this allows outside components to connect to this terminal
    bitInputs << input; // this makes sure terminal and connection positions are updated correctly
    if (initialized) node->Initialize();
    resize();

    return node;
}

void BlockComponent::removeBlockBitInput()
{
    // delete the topmost input node
    int numberInputNodes = bitInputNodes.size();
    if (numberInputNodes <= 0) return;

    BlockNodeComponent * node = bitInputNodes.takeLast(); // always takes last
    mywindow->RemoveComponent(node, false);
    delete node;
    delete bitInputs.takeLast();
    delete blockBitOutputs.takeLast();
    resize();
}

Component * BlockComponent::addBlockBitOutput()
{
    int numberOutputNodes = bitOutputNodes.size();
    if (numberOutputNodes >= 20) return 0;

    BitBlockNodeComponent * node = new BitBlockNodeComponent(numberOutputNodes,false);
    node->createTerminals();
    myscene->components << node;
    setChildComponent(node);
    bitOutputNodes << node;

    InputTerminal * input = node->bitInputs.first();
    input->setParentItem(blockPanel);
    blockBitInputs << input;

    BitOutputTerminal * output = node->bitOutputs.first();
    output->setParentItem(this);
    output->component = this;
    bitOutputs << output;
    if (initialized) node->Initialize();
    resize();

    return node;
}

void BlockComponent::removeBlockBitOutput()
{
    int numberOutputNodes = bitOutputNodes.size();
    if (numberOutputNodes <= 0) return;

    BlockNodeComponent * node = bitOutputNodes.takeLast();
    mywindow->RemoveComponent(node, false);
    delete node;
    delete bitOutputs.takeLast();
    delete blockBitInputs.takeLast();
    resize();
}

Component * BlockComponent::addBlockByteInput()
{
    int numberInputNodes = byteInputNodes.size();
    if (numberInputNodes >= 20) return 0;

    ByteBlockNodeComponent * node = new ByteBlockNodeComponent(numberInputNodes,true);
    node->createTerminals();
    myscene->components << node;
    setChildComponent(node);
    byteInputNodes << node;

    // since this is an input node, the output terminal should be child of the block panel
    OutputTerminal * output = node->byteOutputs.first();
    output->setParentItem(blockPanel);
    blockByteOutputs << output;

    // and the input should be a child of this component
    ByteInputTerminal * input = node->byteInputs.first();
    input->setParentItem(this);
    input->component = this; // this allows outside components to connect to this terminal
    byteInputs << input; // this makes sure terminal and connection positions are updated correctly
    if (initialized) node->Initialize();
    resize();

    return node;
}

void BlockComponent::removeBlockByteInput()
{
    int numberInputNodes = byteInputNodes.size();
    if (numberInputNodes <= 0) return;

    BlockNodeComponent * node = byteInputNodes.takeLast();
    mywindow->RemoveComponent(node, false);
    delete node;
    delete byteInputs.takeLast();
    delete blockByteOutputs.takeLast();
    resize();
}

Component * BlockComponent::addBlockByteOutput()
{
    int numberOutputNodes = byteOutputNodes.size();
    if (numberOutputNodes >= 20) return 0;

    ByteBlockNodeComponent * node = new ByteBlockNodeComponent(numberOutputNodes,false);
    node->createTerminals();
    myscene->components << node;
    setChildComponent(node);
    byteOutputNodes << node;

    InputTerminal * input = node->byteInputs.first();
    input->setParentItem(blockPanel);
    blockByteInputs << input;

    ByteOutputTerminal * output = node->byteOutputs.first();
    output->setParentItem(this);
    output->component = this;
    byteOutputs << output;
    if (initialized) node->Initialize();
    resize();

    return node;
}

void BlockComponent::removeBlockByteOutput()
{
    int numberOutputNodes = byteOutputNodes.size();
    if (numberOutputNodes <= 0) return;

    BlockNodeComponent * node = byteOutputNodes.takeLast();
    mywindow->RemoveComponent(node, false);
    delete node;
    delete byteOutputs.takeLast();
    delete blockByteInputs.takeLast();
    resize();
}

void BlockComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // default appearance
    if (option->state & QStyle::State_Selected)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::black);

    painter->setPen(pen);
    painter->drawRect(rect);
    painter->fillRect(rect,QBrush(QColor(Qt::white)));
    if (name.isEmpty())
        painter->drawPixmap(-16,-16,32,32,*image); // assuming image will be 16x16
    else
        painter->drawText(rect,Qt::AlignCenter,name);
}

void BlockComponent::nameChanged(QString _name)
{
   name = _name;
}
