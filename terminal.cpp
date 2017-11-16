#include "terminal.h"
#include <QString>
#include "component.h"

Component *Terminal::getParent()
{
    return component->parentComponent;
}

Terminal::Terminal()
{

}

QRectF Terminal::boundingRect() const
{
    return QRectF(-width+5/2,-height+5/2,width+10,height+10);
}

void Terminal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-width/2,-height/2,width,height,image);
}
