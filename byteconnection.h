#ifndef BYTECONNECTION_H
#define BYTECONNECTION_H

#include "connection.h"
#include <QPointer>

class ByteInputTerminal;
class ByteOutputTerminal;

class ByteConnection : public Connection
{
public:
    explicit ByteConnection(ByteInputTerminal * _input, ByteOutputTerminal* _output);
    ~ByteConnection();
    virtual InputTerminal *getInputTerminal() override; // implemented by derivatived - casts terminals to base class for doing non-generic stuff
    virtual OutputTerminal* getOutputTerminal() override;

    QPointer<ByteInputTerminal> inputTerminal = 0;
    QPointer<ByteOutputTerminal> outputTerminal = 0;

    virtual void updateColor() override;
    virtual void connect() override;
    virtual void disconnect() override;
};

#endif // BYTECONNECTION_H
