#ifndef DELAYCOMPONENT_H
#define DELAYCOMPONENT_H

#include "component.h"

class BitInputTerminal;
class ByteInputTerminal;
class BitOutputTerminal;
class ByteOutputTerminal;

class DelayComponent : public Component
{
    Q_OBJECT
public:
    DelayComponent();

    QList<bool> prevValues;
    QList<char> prevValuesByte;

    int state = 0; // bit or byte
    int delay = 1;

    //bookmarks
    BitInputTerminal * bitInput = 0;
    ByteInputTerminal * byteInput = 0;
    BitOutputTerminal * bitOutput = 0;
    ByteOutputTerminal * byteOutput = 0;

    // Component interface
public:
    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
public slots:
    void stateChanged(int _state);
    void delayChanged(int _delay);
};

#endif // DELAYCOMPONENT_H
