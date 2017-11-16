#include "connectionnode.h"
#include <QGraphicsSceneMouseEvent>
#include "connection.h"
#include "mygraphicsscene.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "mainwindow.h"

ConnectionNode::ConnectionNode(Connection * _connection)
{
    connection = _connection;
    myscene->addItem(this);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void ConnectionNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    connection->updatePosition();

    QGraphicsObject::mouseMoveEvent(event);
}

void ConnectionNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        connection->deleteNode(this);

    // QGraphicsObject::mousePressEvent(event); this is bad news
}

void ConnectionNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mywindow->resetBlockPanels();
    QGraphicsObject::mouseReleaseEvent(event);
}


QRectF ConnectionNode::boundingRect() const
{
    return connection->nodeRect; // no pen so ok
}

void ConnectionNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(connection->nodeBrush);
    painter->drawEllipse(connection->nodeRect);

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(connection->nodeRect);
    }
}
