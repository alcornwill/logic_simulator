#ifndef OUTPUTCOMPONENT_H
#define OUTPUTCOMPONENT_H

#include "component.h"

class BitInputTerminal;
class ByteInputTerminal;
class QGraphicsTextItem;

// displays signals in different human readable forms
// text mode would actually be useful
class OutputComponent : public Component
{
    Q_OBJECT
public:
    OutputComponent();
    // this one may have variable width
    // has a text graphics label in it, to show value (maybe doesn't show icon)

    int state = 0; // 0 = bit, 1 = byte, maybe more? ascii text?

    // not sure these are necessary but whatever
    bool bitValue = false;
    unsigned char byteValue = 0;

    BitInputTerminal * bitInput = 0;
    ByteInputTerminal * byteInput = 0;

    QGraphicsTextItem * label = 0;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
public slots:
      void changeState(int _state);
};

#endif // OUTPUTCOMPONENT_H
