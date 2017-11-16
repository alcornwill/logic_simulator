#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "component.h"

class BitOutputTerminal;
class ByteOutputTerminal;

// user input component
// would be good if it changed color in bit mode as well
class InputComponent : public Component
{
    Q_OBJECT
public:
    InputComponent();

    int state = 0; // 0 = bit, 1 = unsigned int byte, maybe more?

    bool bitValue = false;
    unsigned char byteValue = 0;

    BitOutputTerminal * bitOutput = 0;
    ByteOutputTerminal * byteOutput = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
public slots:
    void changeState(int _state);
};

#endif // INPUTCOMPONENT_H
