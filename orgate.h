#include "logicgate.h"

#ifndef ORGATE_H
#define ORGATE_H

class InputTerminal;
class OutputTerminal;

class OrGate : public LogicGate
{
public:
    OrGate();

    virtual void ReevaluateLogic() override;
};

#endif // ORGATE_H
