#include "flipflopcomponent.h"
#include "myresources.h"
#include "bitinputterminal.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"

FlipFlopComponent::FlipFlopComponent()
{
    combinational = false;

    set = addBitInputTerminal("set");
    input = addByteInputTerminal("input");
    output = addByteOutputTerminal("output");

    image = &myresources->flipflop;
}


void FlipFlopComponent::createSettings()
{
}

QString FlipFlopComponent::SerializeSettings()
{
    return QString(); // doesn't serialize it's value
}

void FlipFlopComponent::DeserializeSettings(QString string)
{
}

void FlipFlopComponent::ReevaluateLogic()
{
    if (set->getValue())
        value = input->getValue();

    output->setValue(value);
}
