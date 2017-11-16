#include "countercomponent.h"
#include "bitinputterminal.h"
#include "byteoutputterminal.h"
#include "myresources.h"
#include "byteinputterminal.h"

CounterComponent::CounterComponent()
{
    combinational = false;

    input = addBitInputTerminal("input");
    set = addBitInputTerminal("set");
    value = addByteInputTerminal("value");
    output = addByteOutputTerminal("output");

    image = &myresources->counter;
}


void CounterComponent::createSettings()
{
}

QString CounterComponent::SerializeSettings()
{
    // we might want to save the count but I dunno
    return QString();
}

void CounterComponent::DeserializeSettings(QString string)
{
}

void CounterComponent::ReevaluateLogic()
{    
    if (input->getValue())
        count++;

    if (set->getValue())
        count = value->getValue();

    output->setValue(count);
}

void CounterComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // reset count
    count = 0;
}
