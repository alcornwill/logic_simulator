#include "deletecommand.h"
#include "deleteconnectioncommand.h"
#include "mainwindow.h"
#include "component.h"
#include <QPointer>

DeleteCommand::DeleteCommand(Component *_component, Component * _parent)
{
    component = _component;
    parent = _parent; // might be zero
    setText("delete component");
}

DeleteCommand::~DeleteCommand()
{
    if (component && component->deleted)
        delete component;
}

void DeleteCommand::undo()
{
    if (component)
        mywindow->AddComponent(component, parent);
}

void DeleteCommand::redo()
{
    if (component)
        mywindow->RemoveComponent(component);
}
