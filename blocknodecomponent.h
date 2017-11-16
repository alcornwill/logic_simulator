#ifndef BLOCKNODECOMPONENT_H
#define BLOCKNODECOMPONENT_H

#include "component.h"

class InputTerminal;
class OutputTerminal;
class BlockComponent;


class BlockNodeComponent : public Component
{
public:
    BlockNodeComponent(int _index, bool _isInput);

//    virtual InputTerminal* getInputTerminal() = 0; // not sure if will need these
//    virtual OutputTerminal* getOutputTerminal() = 0;

    int index = -1; // the index is the index of the node in the node list e.g. input_node_1, input_node_2...
    bool isInput; // if it's not an input it's an output...

    void setBlock(int _index, bool _isInput);

    void resize() override;
    void createSettings() override;
    QString SerializeSettings() override;
    void DeserializeSettings(QString string) override;
    // void ReevaluateLogic() override;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void createTerminals() = 0;
};

#endif // BLOCKNODECOMPONENT_H
