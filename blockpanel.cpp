#include "blockpanel.h"
#include <QBrush>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include "mygraphicsscene.h"
#include "myresources.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QPointF>
#include "mainwindow.h"
#include "ui_mainwindow.h"

BlockPanel::BlockPanel(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) : QGraphicsRectItem(x,y,width,height,parent)
{
    shadowBrush = QBrush(QColor(100,100,100,100));
    QPen peni = pen();
    peni.setWidth(2);
    peni.setJoinStyle(Qt::MiterJoin);
    setPen(peni);
    setData(0,QVariant("blockpanel")); // used to detect a blockpanel when moving components
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsPanel);
    mywindow->blockPanels << this; // sad that we have to do this
}

BlockPanel::~BlockPanel()
{
    if (mywindow) // when the application is exiting this sometimes throws exception if you don't do this check
        mywindow->blockPanels.removeOne(this);
}

void BlockPanel::setDefaultBrush()
{
    setBrush(QBrush(Qt::white));
//    QPen peni = pen();
//    peni.setColor(Qt::black);
//    setPen(peni);
}

void BlockPanel::setItemDropBrush()
{
    setBrush(QBrush(QBrush(Qt::Dense7Pattern)));
//    QPen peni = pen();
//    peni.setColor(Qt::yellow);
//    setPen(peni);
}

QRectF BlockPanel::boundingRect() const
{
    qreal feather = 1; // the pen width
    QRectF rectf = rect();
    return QRectF(rectf.topLeft().x()-feather,
                  rectf.topLeft().y()-feather,
                  rectf.width()+10+feather, // adding shadow offset, hacky
                  rectf.height()+10+feather);
}

void BlockPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // paint shadow
    QRectF shadowRect = rect();
    shadowRect.moveTopLeft(shadowRect.topLeft()+QPointF(10,10));
    painter->fillRect(shadowRect,shadowBrush);// should move by some constant multiplied by height

    painter->setPen(pen());
    painter->drawRect(rect());
    painter->fillRect(rect(),brush());
}

void BlockPanel::bringToFront()
{
    QList<QGraphicsItem*> colliding = collidingItems();
    QGraphicsItem * myTli = topLevelItem();
    for (int i = 0; i < colliding.size(); i++)
    {
        BlockPanel * enemyPanel = static_cast<BlockPanel*>(colliding[i]);
        if (enemyPanel)
        {
            QGraphicsItem * tli = enemyPanel->topLevelItem();
            tli->stackBefore(myTli);
        }
    }

    myscene->update();
}

QVariant BlockPanel::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
        bringToFront();

    return QGraphicsItem::itemChange(change,value);
}

void BlockPanel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // didn't really need to do this...
    QGraphicsItem::hoverEnterEvent(event);
}

void BlockPanel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (state == 1)
    {
        state = 0;
        qApp->restoreOverrideCursor();
    }
    QGraphicsItem::hoverLeaveEvent(event);
}

void BlockPanel::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF cursorPos = event->lastPos();
    QRectF myrect = rect();

    if (cursorPos.x() > -30 + myrect.width() &&
        cursorPos.y() > -30 + myrect.height())
    {
        if (state == 0)
        {
            state = 1;
            qApp->setOverrideCursor(myresources->resizeCursor);
        }
    }
    else if (state == 1)
    {
        state = 0;
        qApp->restoreOverrideCursor();
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void BlockPanel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // deselect everything
    QList<QGraphicsItem*> selected = scene()->selectedItems();
    for (int i = 0; i < selected.size(); i++)
        selected[i]->setSelected(false);

    bringToFront();

    if (state == 1)
    {
        state = 2;
        // RESIZE
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        mywindow->ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsItem::mousePressEvent(event);
}

void BlockPanel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // stop resizing
    if (state == 2)
    {
        state = 0;
        qApp->restoreOverrideCursor();
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        mywindow->ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        blockComponent->resize();
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void BlockPanel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (state == 2)
    {
        QRectF myrect = rect();
        myrect.setBottomRight(event->pos()+QPointF(15,15));
        setRect(myrect);
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void BlockPanel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // "minimize"
    setVisible(false);

    QGraphicsItem::mouseDoubleClickEvent(event);
}
