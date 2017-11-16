#include <QPen>
#include "maincontext.h"
#include "connectiontype.h"
#include <QPointer>
#include <QGraphicsObject>
#include <QPolygonF>

#ifndef CONNECTION_H
#define CONNECTION_H

class InputTerminal;
class OutputTerminal;
class ConnectionNode;

class Connection : public QGraphicsObject, public MainContext
{
public:
    QColor color;
    QPen polygonPen;
    QBrush nodeBrush;
    QRectF nodeRect;

    int state = 0;
    ConnectionNode * movingNode = 0;
    QPolygonF polygon;

    QList<QPointer<ConnectionNode>> nodes; // sad that need QPointers for these

    ConnectionType connectionType;
    virtual InputTerminal* getInputTerminal() = 0; // implemented by derived - casts terminals to base class for doing non-generic stuff
    virtual OutputTerminal* getOutputTerminal() = 0;

    static Connection * highlighted; // the connection that is highlighted, if there is one. a bit messy...
    bool hoverState = false;
    Connection(OutputTerminal *_outputTerminal);
    ~Connection();

    void deleteNode(ConnectionNode * node);

    bool connected = false;
    void updatePolygon();
    void updatePosition(); // automatically set position
    virtual void updateColor();
    virtual void connect(); // these need to be in base class so that undocommands can call them
    virtual void disconnect();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CONNECTION_H
