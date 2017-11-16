#ifndef NANDCOMPONENT_H
#define NANDCOMPONENT_H

#include "logicgate.h"

class NandComponent : public LogicGate
{
public:
    NandComponent();

    virtual void ReevaluateLogic() override;
};

#endif // NANDCOMPONENT_H
