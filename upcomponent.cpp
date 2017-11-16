#include "upcomponent.h"
#include "byteoutputterminal.h"
#include "bitinputterminal.h"
#include "myresources.h"

UpComponent::UpComponent()
{
    // add 1 byte output
    // 8 bit inputs
    output = addByteOutputTerminal("output");
    input1 = addBitInputTerminal("input1");
    input2 = addBitInputTerminal("input2");
    input3 = addBitInputTerminal("input3");
    input4 = addBitInputTerminal("input4");
    input5 = addBitInputTerminal("input5");
    input6 = addBitInputTerminal("input6");
    input7 = addBitInputTerminal("input7");
    input8 = addBitInputTerminal("input8");

    image = &myresources->up;
}


void UpComponent::createSettings()
{
}

QString UpComponent::SerializeSettings()
{
    return QString();
}

void UpComponent::DeserializeSettings(QString string)
{
}

void UpComponent::ReevaluateLogic()
{
    unsigned char value = 0;
    value |= input1->getValue() << 0;
    value |= input2->getValue() << 1;
    value |= input3->getValue() << 2;
    value |= input4->getValue() << 3;
    value |= input5->getValue() << 4;
    value |= input6->getValue() << 5;
    value |= input7->getValue() << 6;
    value |= input8->getValue() << 7;

    output->setValue(value);
}
