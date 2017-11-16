#include "downcomponent.h"
#include "byteinputterminal.h"
#include "bitoutputterminal.h"
#include "myresources.h"

DownComponent::DownComponent()
{
    // 1 byte input
    // 8 bit outputs
    input = addByteInputTerminal("input");
    output1 = addBitOutputTerminal("output1");
    output2 = addBitOutputTerminal("output2");
    output3 = addBitOutputTerminal("output3");
    output4 = addBitOutputTerminal("output4");
    output5 = addBitOutputTerminal("output5");
    output6 = addBitOutputTerminal("output6");
    output7 = addBitOutputTerminal("output7");
    output8 = addBitOutputTerminal("output8");

    image = &myresources->down;
}


void DownComponent::createSettings()
{
}

QString DownComponent::SerializeSettings()
{
    return QString();
}

void DownComponent::DeserializeSettings(QString string)
{
}

void DownComponent::ReevaluateLogic()
{
    unsigned char value = input->getValue();

    output1->setValue((value >> 0) & 1);
    output2->setValue((value >> 1) & 1);
    output3->setValue((value >> 2) & 1);
    output4->setValue((value >> 3) & 1);
    output5->setValue((value >> 4) & 1);
    output6->setValue((value >> 5) & 1);
    output7->setValue((value >> 6) & 1);
    output8->setValue((value >> 7) & 1);
}
