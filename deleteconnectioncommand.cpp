#include "deleteconnectioncommand.h"
#include "connection.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include "mygraphicsscene.h"
#include <QDebug>

DeleteConnectionCommand::DeleteConnectionCommand(Connection *_connection)
{
    connection = _connection;
    setText("delete connection");
}

DeleteConnectionCommand::~DeleteConnectionCommand()
{
    if (connection && !connection->connected)
        delete connection;
}

void DeleteConnectionCommand::undo()
{
    if (connection)
        connection->connect();
}

void DeleteConnectionCommand::redo()
{
    if (connection)
        connection->disconnect();
}
