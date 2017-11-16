#include "inputcomponent.h"
#include <QComboBox>
#include "bitoutputterminal.h"
#include "byteoutputterminal.h"
#include <QInputDialog>
#include <QDebug>
#include "myresources.h"

InputComponent::InputComponent()
{
    bitOutput = addBitOutputTerminal("output");
    image = &myresources->input;
}


void InputComponent::createSettings()
{
    QComboBox * comboBox = CreateComboBoxSetting(this,SLOT(changeState(int)));
    comboBox->addItem("bit mode");
    comboBox->addItem("byte mode");
    comboBox->setCurrentIndex(state);
}

QString InputComponent::SerializeSettings()
{
    // serialize state and value
    return QString::number(state) + "," + QString::number(bitValue) + "," + QString::number(byteValue);
}

void InputComponent::DeserializeSettings(QString string)
{
    QStringList splitString = string.split(',');
    changeState(splitString[0].toInt());

    bitValue = splitString[1].toInt();
    byteValue = splitString[2].toInt();
}

void InputComponent::ReevaluateLogic()
{
    switch (state)
    {
        case 0:
            // bit
            bitOutput->setValue(bitValue);
            break;
        case 1:
            // byte
            byteOutput->setValue(byteValue);
            break;
    }
}

void InputComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    switch (state)
    {
    case 0:
        bitValue = !bitValue; // toggle value
        if (bitValue)
            image = &myresources->inputAlt;
        else
            image = &myresources->input;
        update();
        break;
    case 1:
        bool ok = true;
        int number = QInputDialog::getInt(0, "set value", // will this be a memory leak?
                                             "value:", byteValue, 0, 255, 1
                                             &ok);
        if (ok)
            byteValue = number;
    }

    Component::mouseDoubleClickEvent(event);
}

void InputComponent::changeState(int _state)
{
    if (state == _state) return;

    // remove current output
    switch (state)
    {
        case 0:
            removeBitOutputTerminal();
            bitValue = false;
            break;
        case 1:
            removeByteOutputTerminal();
            byteValue = 0;
            break;
    }

    // add new output
    switch (_state)
    {
        case 0:
            bitOutput = addBitOutputTerminal("output");
            break;
        case 1:
            byteOutput = addByteOutputTerminal("output");
            break;
    }

    state = _state;
}
