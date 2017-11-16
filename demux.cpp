#include "demux.h"
#include "myresources.h"
#include "byteinputterminal.h"
#include "bitoutputterminal.h"
#include <QDebug>

Demux::Demux()
{
    input = addByteInputTerminal("input");

    addExtraOutput();
    addExtraOutput();

    image = &myresources->demux;
}


void Demux::createSettings()
{
    // add extra output
    CreateButtonSetting("Add Output", this,SLOT(addExtraOutput()));
    // remove extra output
    CreateButtonSetting("Remove Output", this, SLOT(removeExtraOutput()));
}

QString Demux::SerializeSettings()
{
    // serialize extra outputs
    return QString::number(bitOutputs.size() -2);
}

void Demux::DeserializeSettings(QString string)
{
    int number = string.toInt();
    for (int i = 0; i < number; i++)
        addExtraOutput();
}

void Demux::ReevaluateLogic()
{
    // set output #input to true
    int number = input->getValue();

    int size = bitOutputs.size();
    if (number > size-1)
        number = 0;

    for (int i = 0; i < size; i++)
        bitOutputs[i]->setValue(number==i);
}

void Demux::addExtraOutput()
{
    int size = bitOutputs.size();
    if (size >= 20) return;

    addBitOutputTerminal("output " + QString::number(size+1));
}

void Demux::removeExtraOutput()
{
    if (bitOutputs.size() <= 2) return;

    removeBitOutputTerminal();
}
