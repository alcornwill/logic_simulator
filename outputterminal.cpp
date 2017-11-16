#include "outputterminal.h"
#include <QGraphicsLineItem>
#include "mygraphicsscene.h"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QPen>
#include "connection.h"

OutputTerminal::OutputTerminal()
{

}

void OutputTerminal::updateWireColors()
{
    QList<Connection*> connections = getConnections();

    for (int i=0; i < connections.size(); i++)
        connections[i]->updateColor();
}

void OutputTerminal::updatePositions()
{
    // update wire positions

    QList<Connection*> connections = getConnections();

    for (int i = 0; i < connections.size(); i++)
        connections[i]->updatePosition();
}

