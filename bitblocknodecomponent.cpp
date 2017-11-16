#include "bitblocknodecomponent.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"

BitBlockNodeComponent::BitBlockNodeComponent(int _index, bool _isInput) : BlockNodeComponent(_index,_isInput)
{

}

void BitBlockNodeComponent::ReevaluateLogic()
{
    // output = input
    output->setValue(input->getValue());
}

void BitBlockNodeComponent::createTerminals()
{
    if (isInput)
    {
        input = addBitInputTerminal("bit input " + QString::number(index+1));
        output = addBitOutputTerminal("bit input " + QString::number(index+1));
    }
    else
    {
        input = addBitInputTerminal("bit output " + QString::number(index+1));
        output = addBitOutputTerminal("bit output " + QString::number(index+1));
    }
}
