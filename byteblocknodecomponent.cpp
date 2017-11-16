#include "byteblocknodecomponent.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"

ByteBlockNodeComponent::ByteBlockNodeComponent(int _index, bool _isInput) : BlockNodeComponent(_index,_isInput)
{

}

void ByteBlockNodeComponent::ReevaluateLogic()
{
    output->setValue(input->getValue());
}

void ByteBlockNodeComponent::createTerminals()
{
    if (isInput)
    {
        input = addByteInputTerminal("byte input " + QString::number(index+1));
        output = addByteOutputTerminal("byte input " + QString::number(index+1));
    }
    else
    {
        input = addByteInputTerminal("byte output " + QString::number(index+1));
        output = addByteOutputTerminal("byte output " + QString::number(index+1));
    }
}
