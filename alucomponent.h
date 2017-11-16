#ifndef ALUCOMPONENT_H
#define ALUCOMPONENT_H

#include "component.h"

class ByteInputTerminal;
class ByteOutputTerminal;

class AluComponent : public Component
{
public:
    AluComponent();

    // short interface
    ByteInputTerminal * firstOperand = 0;
    ByteInputTerminal * secondOperand = 0;
    ByteInputTerminal * opcode = 0;
    ByteOutputTerminal * product = 0;
    // real ALU would have carry or something?

    // I'm assuming we don't need this to do logic operations like AND, OR and NOT because we've got gates...

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // ALUCOMPONENT_H
