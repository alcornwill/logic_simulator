#include "delaycomponent.h"
#include <QComboBox>
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"
#include "myresources.h"

DelayComponent::DelayComponent()
{
    combinational = false;

    bitInput = addBitInputTerminal("input");
    bitOutput = addBitOutputTerminal("output");

    image = &myresources->delay;
}


void DelayComponent::createSettings()
{
    QComboBox * comboBox = CreateComboBoxSetting(this,SLOT(stateChanged(int)));
    comboBox->addItem("Bit mode");
    comboBox->addItem("Byte mode");
    comboBox->setCurrentIndex(state);

    CreateIntSetting("delay",delay,this,SLOT(delayChanged(int)));
}

QString DelayComponent::SerializeSettings()
{
    return QString::number(state) + "," + QString::number(delay);
}

void DelayComponent::DeserializeSettings(QString string)
{
    QStringList split = string.split(',');
    stateChanged(split[0].toInt());
    delayChanged(split[1].toInt());
}

void DelayComponent::ReevaluateLogic()
{
    switch (state)
    {
    case 0:
        if (delay > 1)
        {
            prevValues << bitInput->getValue();
            if (prevValues.size() > delay - 1)
                bitOutput->setValue(prevValues.takeFirst());
        }
        else
            bitOutput->setValue(bitInput->getValue());
        break;
    case 1:
        if (delay > 1)
        {
            prevValuesByte << byteInput->getValue();
            if (prevValuesByte.size() > delay - 1)
                byteOutput->setValue(prevValuesByte.takeFirst());
        }
        else
            byteOutput->setValue(byteInput->getValue());
        break;
    }
}

void DelayComponent::stateChanged(int _state)
{
    if (state == _state) return;

    switch (state)
    {
    case 0:
        // remove bit terminals
        removeBitInputTerminal();
        removeBitOutputTerminal();
        break;
    case 1:
        // remove byte terminals
        removeByteInputTerminal();
        removeByteOutputTerminal();
        break;
    }

    state = _state;

    switch (state)
    {
    case 0:
        // add bit terminal
        bitInput = addBitInputTerminal("input");
        bitOutput = addBitOutputTerminal("output");
        break;
    case 1:
        // add byte terminals
        byteInput = addByteInputTerminal("input");
        byteOutput = addByteOutputTerminal("output");
        break;
    }

    prevValues.clear();
    prevValuesByte.clear();
}

void DelayComponent::delayChanged(int _delay)
{
    if (delay == _delay) return;

    if (_delay < 1) return;
    if (_delay > 100) return; // don't be silly

    delay = _delay;

    prevValues.clear();
    prevValuesByte.clear();
}
