#ifndef COUNTERCOMPONENT_H
#define COUNTERCOMPONENT_H

#include "component.h"

class BitInputTerminal;
class ByteOutputTerminal;

// counts
class CounterComponent : public Component
{
public:
    CounterComponent();

    unsigned char count = 0;

    BitInputTerminal * input = 0;
    BitInputTerminal * set = 0;
    ByteInputTerminal * value = 0;
    ByteOutputTerminal * output = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // COUNTERCOMPONENT_H
