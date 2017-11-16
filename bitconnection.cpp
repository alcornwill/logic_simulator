#include "bitconnection.h"
#include "mainwindow.h"
#include "bitinputterminal.h"
#include "bitoutputterminal.h"
#include "mygraphicsscene.h"
#include "connectionnode.h"
#include <QDebug>

BitConnection::BitConnection(BitInputTerminal *_input, BitOutputTerminal *_output) : Connection(_output)
{
    connectionType = ConnectionType::Bit;
    inputTerminal = _input;
    outputTerminal = _output;
}

BitConnection::~BitConnection()
{
    disconnect();
}

OutputTerminal *BitConnection::getOutputTerminal()
{
    return outputTerminal;
}

InputTerminal *BitConnection::getInputTerminal()
{
    return inputTerminal;
}

void BitConnection::updateColor()
{
    if (!connected) return;
    if (hoverState)
        color = Qt::red;
    else
    {
        if (outputTerminal->getValue())
            color = Qt::yellow;
        else
            color = Qt::black;
    }

    Connection::updateColor();
}

void BitConnection::connect()
{
    if (connected) return;
    connected = true;

    if (inputTerminal)
        inputTerminal->connect(outputTerminal, this);

    if (outputTerminal)
        outputTerminal->connect(this);

    Connection::connect();
}

void BitConnection::disconnect()
{
    if (!connected) return;
    connected = false;

    if (inputTerminal)
        inputTerminal->disconnect();

    if (outputTerminal)
        outputTerminal->disconnect(this);

//    Q_ASSERT(scene());
//    if (scene())
//        myscene->removeItem(this);

    hoverState = false;

    Connection::disconnect();
}
