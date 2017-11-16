#ifndef FLIPFLOPCOMPONENT_H
#define FLIPFLOPCOMPONENT_H

#include "component.h"

class ByteInputTerminal;
class BitInputTerminal;
class ByteOutputTerminal;
class ByteInputTerminal;

// I think I'm just gonna repurpose this as a "register" which is a made up thing, but not rename it
class FlipFlopComponent : public Component
{
public:
    FlipFlopComponent();

    unsigned short int value = 0;

    ByteInputTerminal * input = 0;
    BitInputTerminal * set = 0;
    ByteOutputTerminal * output = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;

public slots:

};

#endif // FLIPFLOPCOMPONENT_H
