#ifndef LOGICGATE_H
#define LOGICGATE_H

#include "component.h"

class OutputTerminal;
class QToolButton;
class BitOutputTerminal;

// abstract component with some extra slots for logic gate related functionality
class LogicGate : public Component
{
    Q_OBJECT
public:
    LogicGate();

    BitOutputTerminal* output; // shortcut to output, speed is important

    QToolButton * addInputButton = 0;
    QToolButton * removeInputButton = 0;

public slots:
    void addExtraInputTerminal();
    void removeExtraInputTerminal();

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
};

#endif // LOGICGATE_H
