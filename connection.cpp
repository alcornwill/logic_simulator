#include "connection.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include "mygraphicsscene.h"
#include <QDebug>
#include "mainwindow.h"
#include "connectionnode.h"
#include <QGraphicsSceneMouseEvent>

Connection * Connection::highlighted = 0;

void Connection::updatePolygon()
{
    prepareGeometryChange();
    polygon.clear();

    polygon << QPointF();
    for (int i = 0; i < nodes.size(); i++)
        polygon << this->mapFromScene(nodes[i]->scenePos());

    polygon << this->mapFromScene(getInputTerminal()->scenePos());
}

Connection::Connection(OutputTerminal * _outputTerminal)
{
    polygonPen.setWidth(2);
    nodeBrush.setStyle(Qt::SolidPattern);
    nodeRect.setRect(-5,-5,10,10);

    // setBoundingRegionGranularity(1); // rect is too stupid

    setParentItem(_outputTerminal);
}

Connection::~Connection()
{
    // delete all nodes
    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i]) delete nodes[i];
}

void Connection::deleteNode(ConnectionNode *node)
{
    nodes.removeOne(node);
    delete node;
    updatePosition();
}

void Connection::updatePosition()
{
    if (!connected) return;

    updatePolygon();
    update();
}

void Connection::updateColor()
{
    nodeBrush.setColor(color);
    polygonPen.setColor(color);

    for (int i = 0; i < nodes.size(); i++)
        nodes[i]->update();

    update();
}

void Connection::connect()
{
    // for each segment and node, set visible
    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i]) nodes[i]->setVisible(true);

    setVisible(true);
}

void Connection::disconnect()
{
    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i]) nodes[i]->setVisible(false);

    setVisible(false);
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // if lmb, create node and segment
    if (event->button() == Qt::LeftButton)
    {
        state = 1;

        movingNode = new ConnectionNode(this);
        // cannot get polygon segment index, so just have to add node to end
        nodes << movingNode;
        // set as sibling of component
        // I guess it works though because if you change the component's parent, the connections will be deleted anyway
        movingNode->setParentItem(parentItem()->parentItem()->parentItem()); // lol
        movingNode->setPos(event->pos()+parentItem()->pos()+parentItem()->parentItem()->pos()); // lmao

        updatePosition();
    }

    //QGraphicsObject::mousePressEvent(event);
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // if creating node, move node
    if (state == 1)
    {
        movingNode->setPos(event->pos()+parentItem()->pos()+parentItem()->parentItem()->pos());
        updatePosition();
    }

    QGraphicsObject::mouseMoveEvent(event);
}

void Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    state = 0;
    movingNode = 0;
    // stop moving node

    QGraphicsObject::mouseReleaseEvent(event);
}


QRectF Connection::boundingRect() const
{
    return polygon.boundingRect().adjusted(-2,-2,2,2); // adjust for pen
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(polygonPen);
    QPainterPath path;
    path.addPolygon(polygon);
    painter->drawPath(path);
}



QPainterPath Connection::shape() const
{
    QPainterPathStroker stroke;
    stroke.setWidth(7);

    QPainterPath path;
    path.addPolygon(polygon);

    return stroke.createStroke(path);;
}
