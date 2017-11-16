#include <QUndoCommand>
#include "maincontext.h"
#include <QPointer>

#ifndef CREATECONNECTIONCOMMAND_H
#define CREATECONNECTIONCOMMAND_H

class Connection;

class CreateConnectionCommand : public QUndoCommand, public MainContext
{
public:
    CreateConnectionCommand(Connection * _connection);
    ~CreateConnectionCommand();
    QPointer<Connection> connection = 0;

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
};

#endif // CREATECONNECTIONCOMMAND_H
