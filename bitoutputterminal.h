#ifndef BITOUTPUTTERMINAL_H
#define BITOUTPUTTERMINAL_H

#include "outputterminal.h"

class BitConnection;
class Connection;

class BitOutputTerminal : public OutputTerminal
{
public:
    virtual QList<Connection*> getConnections() override;
    BitOutputTerminal();
    bool value = false;
    bool getValue() {return value;}
    void setValue(bool _value) {value = _value;}
    QList<BitConnection*> connections;

    void connect(BitConnection* _connection);
    void disconnect(BitConnection * _connection);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BITOUTPUTTERMINAL_H
