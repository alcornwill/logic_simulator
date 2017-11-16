#ifndef BYTEOUTPUTTERMINAL_H
#define BYTEOUTPUTTERMINAL_H

#include "outputterminal.h"

class ByteConnection;
class Connection;

class ByteOutputTerminal : public OutputTerminal
{
public:
    ByteOutputTerminal();
    virtual QList<Connection*> getConnections() override;
    unsigned char value = 0;
    unsigned char getValue() {return value;}
    void setValue(unsigned char _value) {value = _value;}
    QList<ByteConnection*> connections;

    void connect(ByteConnection* _connection);
    void disconnect(ByteConnection * _connection);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BYTEOUTPUTTERMINAL_H
