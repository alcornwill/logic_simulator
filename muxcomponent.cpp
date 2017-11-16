#include "muxcomponent.h"
#include "bitinputterminal.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"
#include "bitoutputterminal.h"
#include <QComboBox>
#include "myresources.h"

MuxComponent::MuxComponent()
{
    s = addByteInputTerminal("switch");

    bitOutput = addBitOutputTerminal("output");
    // starts off in bit mode with 2 inputs
    addExtraInput();
    addExtraInput();

    // the rest is history

    image = &myresources->mux;
}


void MuxComponent::createSettings()
{
    // bit/byte mode
    QComboBox *comboBox = CreateComboBoxSetting(this,SLOT(stateChanged(int)));
    comboBox->addItem("bit mode");
    comboBox->addItem("byte mode");
    comboBox->setCurrentIndex(state);
    // add/remove inputs
    CreateButtonSetting("add input",this,SLOT(addExtraInput()));
    CreateButtonSetting("remove input",this,SLOT(removeExtraInput()));
}

QString MuxComponent::SerializeSettings()
{
    // serialize mode and number of extra input terminals
    int extraInputs = 0;
    if (!state)
        extraInputs = extraBitInputs.size() - 2;
    else
        extraInputs = extraByteInputs.size() - 2;
    return QString::number(state) + "," + QString::number(extraInputs);
}

void MuxComponent::DeserializeSettings(QString string)
{
    QStringList splitString = string.split(',');
    stateChanged(splitString[0].toInt());

    int extraInputs = splitString[1].toInt();

    for (int i = 0; i < extraInputs; i++)
        addExtraInput();
}

void MuxComponent::ReevaluateLogic()
{
    char index = s->getValue();

    int numberInputs = 0;

    switch (state)
    {
    case 0:
        numberInputs = extraBitInputs.size();
        if (index >= numberInputs) return; // "undefined behaviour"
        bitOutput->setValue(extraBitInputs[index]->getValue());
        break;
    case 1:
        numberInputs = extraByteInputs.size();
        if (index >= numberInputs) return;
        byteOutput->setValue(extraByteInputs[index]->getValue());
        break;
    }
}

void MuxComponent::stateChanged(int _state)
{
    if (state == _state) return;

    int numberInputs = 0;

    switch (state)
    {
    case 0:
        // get rid of all binary inputs and binary output
        numberInputs = extraBitInputs.size();
        for (int i = 0; i < numberInputs; i++)
        {
            removeBitInputTerminal();
            extraBitInputs.removeLast();
        }
        removeBitOutputTerminal();
        break;
    case 1:
        // get rid of all byte inputs and byte output
        numberInputs = extraByteInputs.size();
        for (int i = 0; i < numberInputs; i++)
        {
            removeByteInputTerminal();
            extraByteInputs.removeLast();
        }
        removeByteOutputTerminal();
        break;
    }

    state = _state;

    switch (state)
    {
    case 0:
        // create 2 binary inputs and a binary output
        bitOutput = addBitOutputTerminal("output");
        break;
    case 1:
        // create 2 byte inputs and a byte output
        byteOutput = addByteOutputTerminal("output");
        break;
    }

    addExtraInput();
    addExtraInput();
}

void MuxComponent::addExtraInput()
{
    int numberInputs = 0;
    switch (state)
    {
    case 0:
        numberInputs = extraBitInputs.size();
        if (numberInputs >= 10) return;
        extraBitInputs << addBitInputTerminal("input " + QString::number(numberInputs+1));
        break;

    case 1:
        numberInputs = extraByteInputs.size();
        if (numberInputs >= 10) return;
        extraByteInputs << addByteInputTerminal("input " + QString::number(numberInputs+1));
        break;
    }
}

void MuxComponent::removeExtraInput()
{
    int numberInputs = 0;
    switch (state)
    {
    case 0:
        numberInputs = extraBitInputs.size();
        if (numberInputs <= 2) return;
        removeBitInputTerminal();
        extraBitInputs.removeLast();
        break;
    case 1:
        numberInputs = extraByteInputs.size();
        if (numberInputs <= 2) return;
        removeByteInputTerminal();
        extraByteInputs.removeLast();
        break;
    }
}
