#include <QUndoCommand>
#include "maincontext.h"
#include <QPointer>

#ifndef CREATECOMMAND_H
#define CREATECOMMAND_H

class Component;

class CreateCommand : public QUndoCommand, public MainContext
{
public:
    CreateCommand(Component *_component, Component * _parent);
    ~CreateCommand();
    QPointer<Component> component = 0;
    QPointer<Component> parent = 0;

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
};

#endif // CREATECOMMAND_H
