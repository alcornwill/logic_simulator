#include "orgate.h"
#include <QPixmap>
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

OrGate::OrGate()
{
    image = &myresources->orgate;
}

void OrGate::ReevaluateLogic()
{
    // calculate output based on inputs
    bool out = false;
    for (int i = 0; i < bitInputs.size(); i++)
        out = out || bitInputs[i]->getValue();

    output->setValue(out);
}

