#include "alucomponent.h"
#include "myresources.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"

AluComponent::AluComponent()
{
    opcode = addByteInputTerminal("opcode");
    firstOperand = addByteInputTerminal("first operand");
    secondOperand = addByteInputTerminal("second operand");
    product = addByteOutputTerminal("product");

    image = &myresources->alu;
}

void AluComponent::createSettings()
{
}

QString AluComponent::SerializeSettings()
{
    return QString();
}

void AluComponent::DeserializeSettings(QString string)
{
}

void AluComponent::ReevaluateLogic()
{
    // depending on opcode, do lots of things...
    // same as instructions
    switch (opcode->getValue())
    {
    case 1:
        // add
        product->setValue(firstOperand->getValue() + secondOperand->getValue());
        break;
    case 2:
        // subtract
        product->setValue(firstOperand->getValue() - secondOperand->getValue());
        break;
    case 3:
        // divide
        product->setValue(firstOperand->getValue() / secondOperand->getValue()); // bit weird because it's integers but whatever
        break;
    case 4:
        // multiply
        product->setValue(firstOperand->getValue() * secondOperand->getValue());
        break;
    case 5:
        // modulus
        product->setValue(firstOperand->getValue() % secondOperand->getValue());
        break;
    case 6:
        // shift left
        product->setValue(firstOperand->getValue() << secondOperand->getValue());
        break;
    case 7:
        // shift right
        product->setValue(firstOperand->getValue() >> secondOperand->getValue());
        break;
    case 8:
        // bitwise or
        product->setValue(firstOperand->getValue() | secondOperand->getValue());
        break;
    case 9:
        // bitwise and
        product->setValue(firstOperand->getValue() & secondOperand->getValue());
        break;
    case 10:
        // bitwise xor
        product->setValue(firstOperand->getValue() ^ secondOperand->getValue());
    }
}
