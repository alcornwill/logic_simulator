#include <QGraphicsScene>
#include <QList>
#include "component.h"
#include "maincontext.h"
#include "connectiontype.h"

#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

class QGraphicsSceneMouseEvent;
class QPointF;
class QObject;
class BitOutputTerminal;
class BitInputTerminal;
class ByteOutputTerminal;
class ByteInputTerminal;
class QUndoStack;

class MyGraphicsScene : public QGraphicsScene, public MainContext
{
public:
    explicit MyGraphicsScene(QObject *parent = 0);

    enum Tools {None, MakeComponent};
    Tools activeTool = Tools::None; // the currently active tool
    ComponentType activeComponent = ComponentType::tAndGate; // when the activeTool is MakeComponent, this holds the current type of component being painted
    QList<Component*> components; // list of all components (should be QHash with ID? would be slower in some ways...)
    QUndoStack *undoStack;
    BitOutputTerminal * bitOutputTerminal = 0;
    ByteOutputTerminal * byteOutputTerminal = 0;
    bool grabbedWireState = false;
    ConnectionType wireType;
    QGraphicsLineItem *wire;
    QPointF wireStart;
    Component * previewComponent = 0; // the transparent component you get before you place one
    qreal scale = 1;

    void CancelWire();
    void deletePreviewComponent();
    Component * findComponentWithId(int id);
    Component * findComponentWithOldId(int id);
    void createPreviewComponent(ComponentType type, QPointF mousePoint = QPointF());
    Component * createComponent(ComponentType type); // this should really be private
    Component * placeComponent(ComponentType type, QPointF mousePoint);
    Component * deserializeComponent(ComponentType type, QPointF position);
    void createBitWire(BitOutputTerminal *_bitOutputTerminal);
    void createByteWire(ByteOutputTerminal *_byteOutputTerminal);
    bool createWire(); // private
    void createBitConnection(BitInputTerminal *_bitInputTerminal);
    void createByteConnection(ByteInputTerminal *_byteInputTerminal);
    bool createConnection(InputTerminal *_inputTerminal, OutputTerminal *_outputTerminal); // private

    void initializeComponent(Component *newComponent, QPointF mousePoint, ComponentType type, Component *parent);

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};

#endif // MYGRAPHICSSCENE_H
