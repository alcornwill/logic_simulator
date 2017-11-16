#include "logicgate.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include <QToolButton>

LogicGate::LogicGate()
{
    // create terminals
    addBitInputTerminal("Operand 1");
    addBitInputTerminal("Operand 2");
    output = addBitOutputTerminal("output");
}

void LogicGate::createSettings()
{
    addInputButton = CreateButtonSetting("Add Input Terminal", this, SLOT(addExtraInputTerminal()));
    removeInputButton = CreateButtonSetting("Remove Input Terminal", this, SLOT(removeExtraInputTerminal()));
    //removeInputButton->setDisabled(true); // starts disabled
}

QString LogicGate::SerializeSettings()
{
    return QString::number(bitInputs.size()-2); // -2 because always start with 2...
}

void LogicGate::DeserializeSettings(QString string)
{
    int number = string.toInt();
    for (int i = 0; i < number; i++)
        addExtraInputTerminal();
}


void LogicGate::addExtraInputTerminal()
{
    if (bitInputs.size() >= 10)
        return;

    addBitInputTerminal(QString("Operand %1").arg(bitInputs.size()+1));
}

void LogicGate::removeExtraInputTerminal()
{
    if (bitInputs.size() <= 2)
        return;

    removeBitInputTerminal();
}
