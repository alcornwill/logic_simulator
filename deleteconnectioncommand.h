#include <QUndoCommand>
#include "maincontext.h"
#include <QPointer>

#ifndef DELETECONNECTIONCOMMAND_H
#define DELETECONNECTIONCOMMAND_H

class Connection;

class DeleteConnectionCommand : public QUndoCommand, public MainContext
{
public:
    DeleteConnectionCommand(Connection * _connection);
    ~DeleteConnectionCommand();
    QPointer<Connection> connection = 0;

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
};

#endif // DELETECONNECTIONCOMMAND_H
