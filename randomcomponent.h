#ifndef RANDOMCOMPONENT_H
#define RANDOMCOMPONENT_H

#include "component.h"

class ByteOutputTerminal;

// generates random output
// cba to do bool mode, not even gonna use it
class RandomComponent : public Component
{
public:
    RandomComponent();

    ByteOutputTerminal * output = 0;

    // Component interface
public:
    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
};

#endif // RANDOMCOMPONENT_H
