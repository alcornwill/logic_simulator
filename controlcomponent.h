#ifndef CONTROLCOMPONENT_H
#define CONTROLCOMPONENT_H

#include "component.h"

class ByteInputTerminal;
class BitOutputTerminal;
class ByteOutputTerminal;

// this component is the control part of a processor
// in real life this might be done with microcode or designed with a HDL, so I don't feel too guilty about doing it all in C++
class ControlComponent : public Component
{
public:
    ControlComponent();


    ByteInputTerminal * opcode = 0;

    BitOutputTerminal * memWrite = 0; // MemWrite (else MemRead)
    BitOutputTerminal * regWrite = 0; // RegWrite
    BitOutputTerminal * aluRegWrite = 0; // ALURegWrite
    BitOutputTerminal * pcWrite = 0; // PCWrite
    BitOutputTerminal * pcRegWrite = 0;
    ByteOutputTerminal * regSource = 0; // instruction memory, data memory or register
    ByteOutputTerminal * memIdxSource = 0; // instruction memory or register (if data memory was a source wouldn't reach stable state)
    ByteOutputTerminal * memDataSource = 0; // instruction memory or register
    ByteOutputTerminal * pcSource = 0; // instruction memory or register
    ByteOutputTerminal * regParam = 0; // param1 or param2


    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // CONTROLCOMPONENT_H
