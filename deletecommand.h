#include <QUndoCommand>
#include "maincontext.h"
#include <QPointer>

#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

class Component;

class DeleteCommand : public QUndoCommand, public MainContext
{
public:
    QPointer<Component> component = 0;
    QPointer<Component> parent = 0;
    DeleteCommand(Component *_component,Component * _parent);
    ~DeleteCommand();

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
};

#endif // DELETECOMMAND_H
