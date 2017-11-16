#ifndef BLOCKPANEL_H
#define BLOCKPANEL_H

#include "maincontext.h"
#include <QGraphicsRectItem>
#include <QBrush>

class Component;

class BlockPanel : public QGraphicsRectItem, public MainContext
{
public:
    // component.cpp will set the change the style of this panel when an item is about to be dropped into it
    explicit BlockPanel(qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0);
    ~BlockPanel();
    Component * blockComponent;

    QRectF shadowRect;
    QBrush shadowBrush;

    // int height = 1; use for shadow? (it would never look that great)

    int state = 0; // 0 = normal, 1 = resize cursor

    // should have controls for resizing (not sure how to do this... could have a child graphics item to handle it)
    // should moving/resizing the block panel add commands to undo stack? i think it would be too much work
    void setDefaultBrush();
    void setItemDropBrush();
    void bringToFront();

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event) override;
};


#endif // BLOCKPANEL_H
