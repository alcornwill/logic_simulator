#include "byteoutputterminal.h"
#include "byteconnection.h"
#include "mygraphicsscene.h"

ByteOutputTerminal::ByteOutputTerminal()
{
    connectionType = ConnectionType::Byte;
    image.load(":/Resources/ByteConnection.png");
}

QList<Connection *> ByteOutputTerminal::getConnections()
{
    QList<Connection*> conns;
    for (int i = 0; i < connections.size(); i++)
        conns << connections[i];

    return conns;
}

void ByteOutputTerminal::connect(ByteConnection *_connection)
{
    connected = true;
    connections << _connection;
}

void ByteOutputTerminal::disconnect(ByteConnection *_connection)
{
    connected = false;
    connections.removeOne(_connection);
}

void ByteOutputTerminal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    myscene->createByteWire(this);

    OutputTerminal::mousePressEvent(event);
}
