#include "byteconnection.h"
#include "byteinputterminal.h"
#include "byteoutputterminal.h"
#include "mainwindow.h"
#include "mygraphicsscene.h"
#include <QDebug>
#include "connectionnode.h"

ByteConnection::ByteConnection(ByteInputTerminal *_input, ByteOutputTerminal *_output) : Connection(_output)
{
    connectionType = ConnectionType::Byte;
    inputTerminal = _input;
    outputTerminal = _output;
}

ByteConnection::~ByteConnection()
{
    disconnect();
}

void ByteConnection::updateColor()
{
    if (!connected) return;
    if (hoverState)
        color = Qt::red;
    else
    {
        unsigned char value = outputTerminal->getValue();
        unsigned char out = 0;
        for (int i = 0; i < 7; i++)
            if ((value >> i) & 1)
                out += 32;

        if ((value >> 7) & 1)
            out += 31;

        color = QColor(0,out,out);

        // basic colouring
        //color = QColor(0,value,value); // conveniently both unsigned char and QColor parameters are int from 0-256 :)
    }

    Connection::updateColor();
}

void ByteConnection::connect()
{
    if (connected) return;
    connected = true;

    if (inputTerminal)
        inputTerminal->connect(outputTerminal, this);

    if (outputTerminal)
        outputTerminal->connect(this);

    Connection::connect();
}

void ByteConnection::disconnect()
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

OutputTerminal *ByteConnection::getOutputTerminal()
{
    return outputTerminal;
}

InputTerminal *ByteConnection::getInputTerminal()
{
    return inputTerminal;
}
