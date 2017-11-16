#ifndef MEMORYCOMPONENT_H
#define MEMORYCOMPONENT_H

#include "component.h"
#include <QFont>

class ByteInputTerminal;
class BitInputTerminal;
class ByteOutputTerminal;
class QByteArray;

// this component is used as non-volatile memory or an instruction memory
// since the contents is displayed, it can be used a "computer screen" too.
// the data is edited as text, and can be assembled automatically into machine code

// I kind of need to have line numbers if in assembly mode
class MemoryComponent : public Component
{
    Q_OBJECT
public:
    MemoryComponent();

    // might as well be boolean right? display as a checkbox
    int state = 0; // display mode OR hex mode
    bool assemble = false;
    QByteArray data;
    QByteArray displayData; // this is just data but with some characters replaced with nbsp
    QByteArray machineCode;

    short int size = 1024; // in bytes. should never change


    ByteInputTerminal * dataIn = 0;
    BitInputTerminal * readWrite = 0; // 0 = read, 1 = write
    ByteInputTerminal * index = 0;
    ByteOutputTerminal * dataOut = 0;

    // these are only used in assemble mode
    ByteOutputTerminal * opcode = 0;
    ByteOutputTerminal * param1 = 0;
    ByteOutputTerminal * param2 = 0;
    ByteOutputTerminal * param3 = 0;

    QFont myfont;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setData(QByteArray _data);
    char validateChar(char _char);
    void assembleData();
    QList<QByteArray> removeEmpty(QList<QByteArray> _list);
    void updateDisplayData();

public slots:
    void stateChanged(int _state);
    void assembleChanged(int _assemble);
};

#endif // MEMORYCOMPONENT_H
