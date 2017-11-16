#include "byteinputterminal.h"
#include "outputterminal.h"
#include "connection.h"
#include "byteoutputterminal.h"
#include "byteconnection.h"
#include "mygraphicsscene.h"

ByteInputTerminal::ByteInputTerminal()
{
    connectionType = ConnectionType::Byte;
    image.load(":/Resources/ByteConnection.png");
}

ByteInputTerminal::~ByteInputTerminal()
{
    if (connected && connection) delete connection;
}

OutputTerminal *ByteInputTerminal::getOutputTerminal()
{
    return outputTerminal;
}

Connection *ByteInputTerminal::getConnection()
{
    return connection;
}

unsigned char ByteInputTerminal::getValue()
{
    if (connected)
        return outputTerminal->getValue();
    else return 0; // hopefully this will convert to unsigned char and equal "00000000"
}

void ByteInputTerminal::connect(ByteOutputTerminal *_outputTerminal, ByteConnection *_connection)
{
    connected = true;
    outputTerminal = _outputTerminal;
    connection = _connection;
}

void ByteInputTerminal::disconnect()
{
    connected = false;
    outputTerminal = 0;
    connection = 0;
}

void ByteInputTerminal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    myscene->createByteConnection(this);

    InputTerminal::mousePressEvent(event);
}
