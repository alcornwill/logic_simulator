#include "bitoutputterminal.h"
#include "bitconnection.h"
#include "mygraphicsscene.h"

QList<Connection *> BitOutputTerminal::getConnections()
{
    QList<Connection*> conns;
    for (int i = 0; i < connections.size(); i++)
        conns << connections[i];

    return conns;
}

BitOutputTerminal::BitOutputTerminal()
{
    connectionType = ConnectionType::Bit;
    image.load(":/Resources/Connection.png");
}

void BitOutputTerminal::connect(BitConnection *_connection)
{
    connected = true;
    connections << _connection;
}

void BitOutputTerminal::disconnect(BitConnection *_connection)
{
    connected = false;
    connections.removeOne(_connection);
}

void BitOutputTerminal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // try to create a wire
    myscene->createBitWire(this);

    OutputTerminal::mousePressEvent(event);
}
