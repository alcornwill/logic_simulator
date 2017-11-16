#include "randomcomponent.h"
#include "byteoutputterminal.h"
#include <random>
#include "myresources.h"

RandomComponent::RandomComponent()
{
    combinational = false;

    output = addByteOutputTerminal("output");

    image = &myresources->random;
}


void RandomComponent::createSettings()
{
}

QString RandomComponent::SerializeSettings()
{
    return QString();
}

void RandomComponent::DeserializeSettings(QString string)
{
}

void RandomComponent::ReevaluateLogic()
{
    output->setValue(rand() % 255);
}
