#ifndef BITCONNECTION_H
#define BITCONNECTION_H

#include "connection.h"
#include <QPointer>

class BitInputTerminal;
class BitOutputTerminal;

class BitConnection : public Connection
{
public:
    explicit BitConnection(BitInputTerminal * _input, BitOutputTerminal* _output);
    ~BitConnection();
    virtual InputTerminal *getInputTerminal() override; // implemented by derived classes - casts terminals to base class for doing non-generic stuff
    virtual OutputTerminal* getOutputTerminal() override;

    QPointer<BitInputTerminal> inputTerminal = 0;
    QPointer<BitOutputTerminal> outputTerminal = 0;

    virtual void updateColor() override;
    virtual void connect() override;
    virtual void disconnect() override;
};

#endif // BITCONNECTION_H
