#include "logicgate.h"

#ifndef ANDGATE_H
#define ANDGATE_H


class AndGate : public LogicGate
{
public:
    AndGate();

    virtual void ReevaluateLogic() override;
};

#endif // ANDGATE_H
