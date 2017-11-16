// A component, e.g. a logic gate or peripheral device
// is abstract

#include <QtGlobal>
#include <QGraphicsItem>
#include "maincontext.h"
#include <QPen>
#include <QPointF>
#include <QPointer>

#ifndef COMPONENT_H
#define COMPONENT_H

class QGraphicsScene;
class QPixmap;
class QRectF;
class InputTerminal;
class OutputTerminal;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class MoveCommand;
class QToolButton;
class BlockPanel;
class BitInputTerminal;
class BitOutputTerminal;
class ByteOutputTerminal;
class ByteInputTerminal;
class QComboBox;
class QCheckBox;
class QLineEdit;
class QSpinBox;

enum ComponentType { tAndGate, tOrGate, tNotGate, tClock, tBlock, tAdder, tInput, tOutput, tBlockNode, tXor, tNor, tNand, tFlipFlop, tMux, tCounter, tMemory, tUp, tDown, tControl, tEquality, tRandom, tAlu, tDelay, tDemux };

class Component : public QGraphicsObject, public MainContext
{
public:

    QList<BitInputTerminal*> bitInputs;
    QList<BitOutputTerminal*> bitOutputs;

    QList<ByteInputTerminal*> byteInputs;
    QList<ByteOutputTerminal*> byteOutputs;

    // should these be static? doesn't really make any difference
    BitInputTerminal * addBitInputTerminal(QString name);
    BitOutputTerminal * addBitOutputTerminal(QString name);
    ByteInputTerminal * addByteInputTerminal(QString name);
    ByteOutputTerminal * addByteOutputTerminal(QString name);
    virtual void resize();

    void removeBitInputTerminal();
    void removeBitOutputTerminal();
    void removeByteInputTerminal();
    void removeByteOutputTerminal();

    QList<InputTerminal *> getInputTerminals();
    QList<OutputTerminal*> getOutputTerminals();

    bool combinational = true; // if a component is not combinational then it will be reevaluated more than once per simulation step to reduce undefined behaviour from loops
    ComponentType type;
    Component();
    QPixmap *image = 0;
    QRectF rect;
    QPen pen;
    int width = 40;
    int height = 40;
    virtual void Initialize(); // some constructor stuff is put in here so that I can have the previews
    bool reevaluated = false;
    int componentID = -1;
    int oldID = -1; // this is used during deserialization and should be reset afterwards
    bool deleted = false; // this is just used by undo commands I think. means deleted from scene, obviously not deleted from memory
    bool fixedHeight = false; // used by memory component

    // component heirarchy
    // unfortunately I can't use just the QT heirarchy, so I've implemented my own one
    QPointer<Component> parentComponent = 0;
    QList<Component *> childComponents;

    Component * startParent = 0;
    Component * endParent = 0;
    QPointF startMove;
    QPointF endMove;
    BlockPanel * endPanel = 0; // a panel that we are about to place ourselves in
    bool hasMoved = false;

    virtual void setChildComponent(Component * component);
    virtual void removeChildComponent(Component * component);
    virtual void createSettings() = 0; // this is called every time a component is selected, it populates the properties panel
    virtual QString SerializeSettings() = 0;
    virtual void DeserializeSettings(QString string) = 0;

    void Reevaluate();
    virtual void ReevaluateLogic() = 0; // the inner logic specific to the component
    void saveStartPos(); // hate this but gather there isn't a better way
    MoveCommand *getMoveCommand();

    BitInputTerminal *findBitInput(QString name);
    BitOutputTerminal *findBitOutput(QString name);
    ByteInputTerminal *findByteInput(QString name);
    ByteOutputTerminal *findByteOutput(QString name);
    void deleteAllConnections();

    QToolButton *CreateButtonSetting(QString name, const QObject *receiver, const char *method);
    QComboBox *CreateComboBoxSetting(const QObject *receiver, const char *method);
    QCheckBox *CreateCheckBoxSetting(QString text, bool value, const QObject *receiver, const char *method);
    QSpinBox *CreateIntSetting(QString text, int number, const QObject *receiver, const char *method);
    QLineEdit *CreateStringSetting(QString text, const QObject *receiver, const char *method);

    // is override?
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    Component *findColliding();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // COMPONENT_H
