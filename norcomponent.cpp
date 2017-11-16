#include "norcomponent.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

NorComponent::NorComponent()
{
    image = &myresources->norgate;
}


void NorComponent::ReevaluateLogic()
{
    bool or = false;
    for (int i = 0; i < bitInputs.size(); i++)
        or = or || bitInputs[i]->getValue();

    // not or
    output->setValue(!or);
}
