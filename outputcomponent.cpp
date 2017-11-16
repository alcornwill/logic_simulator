#include "outputcomponent.h"
#include <QComboBox>
#include <QPainter>
#include "bitinputterminal.h"
#include "byteinputterminal.h"
#include <QStyleOptionGraphicsItem>

OutputComponent::OutputComponent()
{
    bitInput = addBitInputTerminal("input");
}

void OutputComponent::createSettings()
{
    QComboBox *comboBox = CreateComboBoxSetting(this, SLOT(changeState(int)));
    comboBox->addItem("bit mode");
    comboBox->addItem("byte mode");
    comboBox->setCurrentIndex(state);
}

QString OutputComponent::SerializeSettings()
{
    return QString::number(state);
}

void OutputComponent::DeserializeSettings(QString string)
{
    changeState(string.toInt());
}

void OutputComponent::ReevaluateLogic()
{
    switch (state)
    {
    case 0:
        bitValue = bitInput->getValue();
        break;
    case 1:
        byteValue = byteInput->getValue();
        break;
    }

    update();
}

void OutputComponent::changeState(int _state)
{
    if (state == _state) return;

    // remove old terminal
    switch (state)
    {
    case 0:
        removeBitInputTerminal();
        break;
    case 1:
        removeByteInputTerminal();
        break;
    }

    state = _state;

    // add new terminal
    switch (state)
    {
    case 0:
        bitInput = addBitInputTerminal("input");
        break;
    case 1:
        byteInput = addByteInputTerminal("input");
        break;
    }
}

void OutputComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // overrides default paint algorithm because it doesn't have an image
    if (option->state & QStyle::State_Selected)
        pen.setColor(Qt::yellow);
    else
        pen.setColor(Qt::black);

    painter->setPen(pen);
    painter->drawRect(rect);
    painter->fillRect(rect,QBrush(QColor(Qt::white)));
    //painter->drawPixmap(-16,-16,32,32,image); // assuming image will be 16x16
    switch (state)
    {
    case 0:
        painter->drawText(rect,Qt::AlignCenter,QString::number(bitValue));
        break;
    case 1:
        painter->drawText(rect,Qt::AlignCenter,QString::number(byteValue)); // width may need to be increased
        break;
    }
}

