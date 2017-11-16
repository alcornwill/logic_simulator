#ifndef XORCOMPONENT_H
#define XORCOMPONENT_H

#include "logicgate.h"

class XorComponent : public LogicGate
{
public:
    XorComponent();

    virtual void ReevaluateLogic() override;
};

#endif // XORCOMPONENT_H
