// a component can have multiple input and output terminals.
// terminals are connected with connections

#include <QGraphicsObject>
#include <QtGlobal>
#include <QRectF>
#include <QPainter>
#include "maincontext.h"
#include "connectiontype.h"

#ifndef TERMINAL_H
#define TERMINAL_H

class QString;
class QStyleOptionGraphicsItem;
class QWidget;
class QPointF;
class Component;

class Terminal : public QGraphicsObject, public MainContext
{
public:
    QPixmap image;
    Component * getParent(); // get the parent of component
    ConnectionType connectionType = ConnectionType::Bit;
    Terminal();
    Component * component = 0; // the component that this terminal belongs to
    QString name;
    int width = 12;
    int height = 12;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // TERMINAL_H
