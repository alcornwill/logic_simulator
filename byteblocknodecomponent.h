#ifndef BYTEBLOCKNODECOMPONENT_H
#define BYTEBLOCKNODECOMPONENT_H

#include "blocknodecomponent.h"

class ByteBlockNodeComponent : public BlockNodeComponent
{
public:
    ByteBlockNodeComponent(int _index, bool _isInput);

    ByteInputTerminal * input = 0;
    ByteOutputTerminal  * output = 0;

    virtual void ReevaluateLogic() override;
    virtual void createTerminals() override;
};

#endif // BYTEBLOCKNODECOMPONENT_H
