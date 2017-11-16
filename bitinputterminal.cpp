#include "bitinputterminal.h"
#include "mygraphicsscene.h"
#include "bitoutputterminal.h"
#include "bitconnection.h"

BitInputTerminal::BitInputTerminal()
{
    connectionType = ConnectionType::Bit;
    image.load(":/Resources/Connection.png");
}

BitInputTerminal::~BitInputTerminal()
{
    if (connected && connection) delete connection;
}

OutputTerminal *BitInputTerminal::getOutputTerminal()
{
    return outputTerminal;
}

Connection *BitInputTerminal::getConnection()
{
    return connection;
}

bool BitInputTerminal::getValue()
{
    if (connected)
        return outputTerminal->getValue();
    else return false;
}

void BitInputTerminal::connect(BitOutputTerminal *_outputTerminal, BitConnection *_connection)
{
    connected = true;
    outputTerminal = _outputTerminal;
    connection = _connection;
}

void BitInputTerminal::disconnect()
{
    connected = false;
    outputTerminal = 0;
    connection = 0;
}

void BitInputTerminal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    myscene->createBitConnection(this);

    InputTerminal::mousePressEvent(event);
}
