#include "nandcomponent.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

NandComponent::NandComponent()
{
    image = &myresources->nandgate;
}


void NandComponent::ReevaluateLogic()
{
    bool and = true;
    for (int i = 0; i < bitInputs.size(); i++)
        and = and && bitInputs[i]->getValue();

    // not and
    output->setValue(!and);
}
