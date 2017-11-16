#include "equalitycomponent.h"
#include "bitoutputterminal.h"
#include "byteinputterminal.h"
#include "myresources.h"

EqualityComponent::EqualityComponent()
{
    output = addBitOutputTerminal("output");
    input1 = addByteInputTerminal("input 1");
    input2 = addByteInputTerminal("input 2");

    image = &myresources->equality;
}


void EqualityComponent::createSettings()
{
}

QString EqualityComponent::SerializeSettings()
{
    return QString();
}

void EqualityComponent::DeserializeSettings(QString string)
{
}

void EqualityComponent::ReevaluateLogic()
{
    output->setValue(input1->getValue() == input2->getValue());
}
