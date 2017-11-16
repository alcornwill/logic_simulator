#include "blocknodecomponent.h"
#include "inputterminal.h"
#include "outputterminal.h"
#include "blockcomponent.h"
#include <QStringList>
#include <QDebug>

// this component is a cheeky way of implementing block component
// it isn't visible, apart from it's terminals
// it has one input and one output terminal
// one terminal goes outside the block, one goes on the inside

BlockNodeComponent::BlockNodeComponent(int _index, bool _isInput)
{
    type = ComponentType::tBlockNode;

    index = _index;
    isInput = _isInput;
}

void BlockNodeComponent::resize()
{
    // we don't want it to do this
}

void BlockNodeComponent::createSettings()
{
    // doesn't have any I think
}

QString BlockNodeComponent::SerializeSettings()
{
    // this shouldn't be called
    qDebug() << "tried to serialize block node component";
    return QString();
}

void BlockNodeComponent::DeserializeSettings(QString string)
{
    // this shouldn't be called either
    qDebug() << "tried to deserialize block node component";
}

QRectF BlockNodeComponent::boundingRect() const
{
    return QRectF(); // 0,0,0,0
}

void BlockNodeComponent::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     // don't paint
}
