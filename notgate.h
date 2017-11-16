#ifndef NOTGATE_H
#define NOTGATE_H

#include "component.h"

class BitInputTerminal;
class BitOutputTerminal;

class NotGate : public Component
{
public:
    NotGate();

    BitInputTerminal * input = 0;
    BitOutputTerminal * output = 0;

    // Component interface
public:
    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // NOTGATE_H
