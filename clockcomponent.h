#ifndef CLOCKCOMPONENT_H
#define CLOCKCOMPONENT_H

#include "component.h"

class BitOutputTerminal;

// output is a regular pulse, counts to a number (not real-time)
class ClockComponent : public Component
{
public:
    ClockComponent();

    int count = 0;
    int interval = 1;

    BitOutputTerminal * output = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CLOCKCOMPONENT_H
