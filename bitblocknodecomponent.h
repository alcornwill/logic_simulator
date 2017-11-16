#ifndef BITBLOCKNODECOMPONENT_H
#define BITBLOCKNODECOMPONENT_H

#include "blocknodecomponent.h"

class BitBlockNodeComponent : public BlockNodeComponent
{
public:
    BitBlockNodeComponent(int _index, bool _isInput);

    BitInputTerminal * input = 0;
    BitOutputTerminal  * output = 0;

    virtual void ReevaluateLogic() override;
    virtual void createTerminals() override;
};

#endif // BITBLOCKNODECOMPONENT_H
