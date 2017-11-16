#include "createconnectioncommand.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include "connection.h"
#include "mygraphicsscene.h"

CreateConnectionCommand::CreateConnectionCommand(Connection *_connection)
{
    connection = _connection;
    setText("create connection");
}

CreateConnectionCommand::~CreateConnectionCommand()
{
    if (connection && !connection->connected)
        delete connection;
}

void CreateConnectionCommand::undo()
{
    if (connection)
        connection->disconnect();
}

void CreateConnectionCommand::redo()
{
    if (connection)
        connection->connect();
}
