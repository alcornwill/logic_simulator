#ifndef BITINPUTTERMINAL_H
#define BITINPUTTERMINAL_H

#include "inputterminal.h"
#include <QPointer>

class BitOutputTerminal;
class BitConnection;

class BitInputTerminal : public InputTerminal
{
public:
    BitInputTerminal();
    ~BitInputTerminal();
    virtual OutputTerminal * getOutputTerminal() override;
    virtual Connection * getConnection() override;

    virtual bool getValue();

    void connect(BitOutputTerminal * _outputTerminal, BitConnection * _connection);
    void disconnect();
    QPointer<BitOutputTerminal> outputTerminal = 0;
    QPointer<BitConnection> connection = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BITINPUTTERMINAL_H
