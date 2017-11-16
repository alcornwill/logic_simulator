#ifndef DOWNCOMPONENT_H
#define DOWNCOMPONENT_H

#include "component.h"

class ByteInputTerminal;
class BitOutputTerminal;

class DownComponent : public Component
{
public:
    DownComponent();

    ByteInputTerminal * input = 0;
    BitOutputTerminal * output1 = 0;
    BitOutputTerminal * output2 = 0;
    BitOutputTerminal * output3 = 0;
    BitOutputTerminal * output4 = 0;
    BitOutputTerminal * output5 = 0;
    BitOutputTerminal * output6 = 0;
    BitOutputTerminal * output7 = 0;
    BitOutputTerminal * output8 = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // DOWNCOMPONENT_H
