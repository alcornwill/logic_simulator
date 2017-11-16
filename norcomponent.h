#ifndef NORCOMPONENT_H
#define NORCOMPONENT_H

#include "logicgate.h"

class NorComponent : public LogicGate
{
public:
    NorComponent();

    virtual void ReevaluateLogic() override;
};

#endif // NORCOMPONENT_H
