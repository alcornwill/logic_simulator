#ifndef BLOCKCOMPONENT_H
#define BLOCKCOMPONENT_H

#include "component.h"

class BlockNodeComponent;
class BlockPanel;
class InputTerminal;
class OutputTerminal;

class BlockComponent : public Component
{
    Q_OBJECT
public:
    BlockComponent();
    ~BlockComponent();

    QString name = "";
    // BlockComponent doesn't have an input or output terminals itself
    QList<BlockNodeComponent*> bitInputNodes; // input nodes are inputs to the block, outputs inside the block
    QList<BlockNodeComponent*> bitOutputNodes; // output nodes are outputs to the block, inputs inside the block
    QList<BlockNodeComponent*> byteInputNodes;
    QList<BlockNodeComponent*> byteOutputNodes;

    QList<InputTerminal*> blockBitInputs; // the terminals that are children of the block panel
    QList<OutputTerminal*> blockBitOutputs;
    QList<InputTerminal*> blockByteInputs;
    QList<OutputTerminal*> blockByteOutputs;

    BlockPanel * blockPanel; // any children of this block should actually be children of this panel    
    bool initialized = false;

    virtual void ReevaluateLogic() override;

    virtual void setChildComponent(Component * component) override;
    virtual void removeChildComponent(Component *component) override;
    virtual void resize() override;
    virtual void Initialize() override;
    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
public slots:

    void nameChanged(QString _name);
    Component * addBlockBitInput();
    void removeBlockBitInput();
    Component * addBlockBitOutput();
    void removeBlockBitOutput();
    Component * addBlockByteInput();
    void removeBlockByteInput();
    Component * addBlockByteOutput();
    void removeBlockByteOutput();
};

#endif // BLOCKCOMPONENT_H
