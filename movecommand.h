#include <QUndoCommand>
#include <QPointF>

#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

class Component;

class MoveCommand : public QUndoCommand
{
public:
    Component *component = 0;
    Component *newParent = 0;
    Component *oldParent = 0;
    QPointF myOldPos;
    QPointF myNewPos;
    //enum { Id = 1234 };

    MoveCommand(Component *_component, Component *_newParent, Component *_oldParent, QPointF &oldPos, QPointF &newPos);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;
    //bool mergeWith(const QUndoCommand *command) Q_DECL_OVERRIDE;
    //int id() const Q_DECL_OVERRIDE { return Id; }
};

#endif // MOVECOMMAND_H
