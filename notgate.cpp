#include "notgate.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

NotGate::NotGate()
{
    input = addBitInputTerminal("input");
    output = addBitOutputTerminal("output");

    image = &myresources->notgate;
}


void NotGate::createSettings()
{
}

QString NotGate::SerializeSettings()
{
    return QString();
}

void NotGate::DeserializeSettings(QString string)
{
}

void NotGate::ReevaluateLogic()
{
    output->setValue(!input->getValue());
}
