#include "clockcomponent.h"
#include "myresources.h"
#include <QInputDialog>
#include "bitoutputterminal.h"

ClockComponent::ClockComponent()
{
    combinational = false;

    output = addBitOutputTerminal("output");

    image = &myresources->clock;
}


void ClockComponent::createSettings()
{
    // no settings
}

QString ClockComponent::SerializeSettings()
{
    return QString::number(interval);
}

void ClockComponent::DeserializeSettings(QString string)
{
    interval = string.toInt();
}

void ClockComponent::ReevaluateLogic()
{
    count++;
    if (count > interval)
    {
        output->setValue(true);
        count = 0;
    }
    else
        output->setValue(false);
}

void ClockComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    bool ok = true;
    int number = QInputDialog::getInt(0, "set value",
                                         "value:", interval, 1, 1000, 1
                                         &ok);
    if (ok)
        interval = number;

    Component::mouseDoubleClickEvent(event);
}
