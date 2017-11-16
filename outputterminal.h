#include "terminal.h"
#include <QList>

#ifndef OUTPUTTERMINAL_H
#define OUTPUTTERMINAL_H

class QGraphicsLineItem;
class Connection;

class OutputTerminal : public Terminal
{
public:
    virtual QList<Connection*> getConnections() = 0; // connection list is implemented in derived member
    OutputTerminal();

    bool connected = false;
    void updateWireColors();
    void updatePositions();
};

#endif // OUTPUTTERMINAL_H
