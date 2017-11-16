#include "andgate.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

AndGate::AndGate()
{
    image = &myresources->andgate;
}

void AndGate::ReevaluateLogic()
{
    bool out = true;
    for (int i = 0; i < bitInputs.size(); i++)
        out = out && bitInputs[i]->getValue();

    output->setValue(out);
}
