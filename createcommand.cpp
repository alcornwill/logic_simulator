#include "createcommand.h"
#include "mygraphicsscene.h"
#include "mainwindow.h"

CreateCommand::CreateCommand(Component *_component, Component *_parent)
{
    component = _component;
    parent = _parent;
    setText("create component");
}

CreateCommand::~CreateCommand()
{
    // create/delete commands can hold the last reference to a component that was removed from the scene
    // so when the command is deleted, we may need to delete the component or there will be a memory leak
    // yes this would probably be solvable with shared pointers, but shared pointers
    // can still have dangling pointers. That's why QPointer is cool.

    // the component may have already been deleted by the QT parenting memory management system
    //Q_ASSERT(component);

    if (component && component->deleted)
        delete component;
}

void CreateCommand::undo()
{
    Q_ASSERT(component);
    // delete component
    if (component)
        mywindow->RemoveComponent(component);
    // if the component doesn't exist anymore then this command will sit in the stack and do nothing...
}

void CreateCommand::redo()
{
    Q_ASSERT(component);
    if (component)
        mywindow->AddComponent(component, parent);
}
