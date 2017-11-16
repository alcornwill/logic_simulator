#ifndef UPCOMPONENT_H
#define UPCOMPONENT_H

#include "component.h"

class BitInputTerminal;
class ByteOutputTerminal;

class UpComponent : public Component
{
public:
    UpComponent();

    ByteOutputTerminal *output = 0;
    BitInputTerminal *input1 = 0;
    BitInputTerminal *input2 = 0;
    BitInputTerminal *input3 = 0;
    BitInputTerminal *input4 = 0;
    BitInputTerminal *input5 = 0;
    BitInputTerminal *input6 = 0;
    BitInputTerminal *input7 = 0;
    BitInputTerminal *input8 = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // UPCOMPONENT_H
