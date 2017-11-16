#ifndef MUXCOMPONENT_H
#define MUXCOMPONENT_H

#include "component.h"

class ByteInputTerminal;
class ByteOutputTerminal;
class BitOutputTerminal;

class MuxComponent : public Component
{
    Q_OBJECT
public:
    MuxComponent();

    int state = 0;

    ByteInputTerminal * s = 0;
    ByteOutputTerminal * byteOutput = 0;
    BitOutputTerminal * bitOutput = 0;
    QList<BitInputTerminal*> extraBitInputs;
    QList<ByteInputTerminal*> extraByteInputs;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
public slots:
    void stateChanged(int _state);
    void addExtraInput();
    void removeExtraInput();
};

#endif // MUXCOMPONENT_H
