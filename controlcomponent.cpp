#include "controlcomponent.h"
#include "myresources.h"
#include "byteinputterminal.h"
#include "bitoutputterminal.h"
#include "byteoutputterminal.h"

ControlComponent::ControlComponent()
{
    opcode = addByteInputTerminal("opcode");

    memWrite = addBitOutputTerminal("MemWrite");
    regWrite = addBitOutputTerminal("RegWrite");
    aluRegWrite = addBitOutputTerminal("ALURegWrite");
    pcWrite = addBitOutputTerminal("PCWrite");
    pcRegWrite = addBitOutputTerminal("PCRegWrite");
    regSource = addByteOutputTerminal("RegSource");
    memIdxSource = addByteOutputTerminal("MemIdxSource");
    memDataSource = addByteOutputTerminal("MemDataSource");
    pcSource = addByteOutputTerminal("PCSource");
    regParam = addByteOutputTerminal("RegParam");

    image = &myresources->control;
}

void ControlComponent::createSettings()
{
    // don't think there are any settings
}

QString ControlComponent::SerializeSettings()
{
    // don't think there's anything to serialize either
    return QString();
}

void ControlComponent::DeserializeSettings(QString string)
{
}

void ControlComponent::ReevaluateLogic()
{
    // switch on opcode to sequence the processor
    unsigned char value = opcode->getValue();

    bool _aluRegWrite = false;
    bool _memWrite = false;
    bool _regWrite = false;
    bool _pcWrite = false;
    bool _pcRegWrite = false;
    char _regSrc = 0;
    char _memIdxSrc = 0;
    char _memDataSrc = 0;
    char _pcSrc = 0;
    char _regParam = 0;

    // kind of reluctant to hard code these because opcodes are subject to change
    switch (value)
    {
    case 0:
        // no op

        break;
    // logic arithmetic
    case 1:
        // add
        _aluRegWrite = true;
        break;
    case 2:
        // subtract
        _aluRegWrite = true;
        break;
    case 3:
        // divide
        _aluRegWrite = true;
        break;
    case 4:
        // multiply
        _aluRegWrite = true;
        break;
    case 5:
        // shift left
        _aluRegWrite = true;
        break;
    case 6:
        // shift right
        _aluRegWrite = true;
        break;
    case 7:
        // and
        _aluRegWrite = true;
        break;
    case 8:
        // or
        _aluRegWrite = true;
        break;
    case 9:
        // not
        _aluRegWrite = true;
        break;
    case 10:
        // nor
        _aluRegWrite = true;
        break;
    case 11:
        // xor
        _aluRegWrite = true;
        break;

    // control structure
    case 12:
        // branch if equal

        break;
    case 13:
        // branch if not equal

        break;
    case 14:
        // branch if less than

        break;
    case 15:
        // branch if greater than

        break;
    case 16:
        // jump to line
        _pcWrite = true;
        _pcSrc = 0; // instruction memory
        break;
    case 17:
        // jump and link
        _pcWrite = true;
        _pcSrc = 0; // instruction memory
        _pcRegWrite = true;
        break;
    case 18:
        // jump to register
        _pcWrite = true;
        _pcSrc = 1; // register
        break;

    // memory operations
    case 19:
        // load
        _regWrite = true;
        _memIdxSrc = 1; // register
        _regSrc = 1; // data memory
        _regParam = 1; // param2
        break;
    case 20:
        // load intermediate
        _regWrite = true;
        _regSrc = 0; // instruction memory
        break;
    case 21:
        // store
        _memWrite = true;
        _memDataSrc = 1; // register
        _memIdxSrc = 1; // register
        break;
    case 22:
        // store intermediate
        _memWrite = true;
        _memDataSrc = 0; // instruction memory
        _memIdxSrc = 1; // register
        break;
    case 23:
        // move
        _regWrite = true;
        _regSrc= 2; // register
        break;
    }

    memWrite->setValue(_memWrite);
    regWrite->setValue(_regWrite);
    aluRegWrite->setValue(_aluRegWrite);
    pcWrite->setValue(_pcWrite);
    pcRegWrite->setValue(_pcRegWrite);
    regSource->setValue(_regSrc);
    memIdxSource->setValue(_memIdxSrc);
    memDataSource->setValue(_memDataSrc);
    pcSource->setValue(_pcSrc);
    regParam->setValue(_regParam);
}
