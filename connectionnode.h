#ifndef CONNECTIONNODE_H
#define CONNECTIONNODE_H

#include <QGraphicsObject>
#include "maincontext.h"

class Connection;
class ConnectionSegment;

class ConnectionNode : public QGraphicsObject, public MainContext
{
public:
    ConnectionNode(Connection * _connection);

    Connection * connection = 0; // the connection class handles everything

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CONNECTIONNODE_H
