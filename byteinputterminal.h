#ifndef BYTEINPUTTERMINAL_H
#define BYTEINPUTTERMINAL_H

#include "inputterminal.h"
#include <QPointer>

class ByteOutputTerminal;
class ByteConnection;
class OutputTerminal;
class Connection;

class ByteInputTerminal : public InputTerminal
{
public:
    ByteInputTerminal();
    ~ByteInputTerminal();

    virtual OutputTerminal * getOutputTerminal() override;
    virtual Connection * getConnection() override;

    virtual unsigned char getValue();

    void connect(ByteOutputTerminal * _outputTerminal, ByteConnection * _connection);
    void disconnect();
    QPointer<ByteOutputTerminal> outputTerminal = 0;
    QPointer<ByteConnection> connection = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BYTEINPUTTERMINAL_H
