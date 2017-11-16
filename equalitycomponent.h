#ifndef EQUALITYCOMPONENT_H
#define EQUALITYCOMPONENT_H

#include "component.h"

class BitOutputTerminal;
class ByteInputTerminal;

// checks if two byte values are equal
class EqualityComponent : public Component
{
public:
    EqualityComponent();

    BitOutputTerminal * output = 0;
    ByteInputTerminal * input1 = 0;
    ByteInputTerminal * input2 = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // EQUALITYCOMPONENT_H
