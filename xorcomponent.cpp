#include "xorcomponent.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

XorComponent::XorComponent()
{
    image = &myresources->xorgate;
}


void XorComponent::ReevaluateLogic()
{
    // OR AND NOT AND
    bool or = false;
    for (int i = 0; i < bitInputs.size(); i++)
        or = or || bitInputs[i]->getValue();

    bool and = true;
    for (int i = 0; i < bitInputs.size(); i++)
        and = and && bitInputs[i]->getValue();

    output->setValue(or && !and);
}
