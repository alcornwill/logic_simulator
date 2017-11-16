#include "movecommand.h"
#include "component.h"
#include <QPointF>
#include <QDebug>

MoveCommand::MoveCommand(Component *_component, Component *_newParent, Component* _oldParent,QPointF &oldPos, QPointF &newPos)
{
    component = _component;
    newParent = _newParent;
    oldParent = _oldParent;
    myNewPos = newPos;
    myOldPos = oldPos;
    setText("move component");
}

void MoveCommand::undo()
{
    if (!component) return;

    if (newParent) newParent->removeChildComponent(component);
    if (oldParent) oldParent->setChildComponent(component);
    else
    {
        component->setParentItem(0); // make it top level
        component->parentComponent= 0;
    }

    component->setPos(myOldPos);
}

void MoveCommand::redo()
{
    if (!component) return;

    if (oldParent) oldParent->removeChildComponent(component);
    if (newParent) newParent->setChildComponent(component);
    else
    {
        component->setParentItem(0);
        component->parentComponent= 0;
    }

    component->setPos(myNewPos);
}
