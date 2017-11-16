#include <QMainWindow>
#include "maincontext.h"
#include <QButtonGroup>
#include <QToolButton>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QString;
class QGraphicsScene;
class MyGraphicsScene;
class QLayout;
class Component;
class QTextStream;
class QFileSystemModel;
class QTimer;
class BlockPanel;
class lua_State;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MainContext
{
    Q_OBJECT
    // might move some of this stuff to MainContext?
    QString currentSaveFile = "";

    // maybe if simulation frequency goes below 10ms then it should stop updating wire colours
    int simulationFrequency = 1000;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QButtonGroup buttonGroup;
    QToolButton noneButton;
    Ui::MainWindow *ui;
    int nextFreeComponentId = 0;
    QString header = "LogicSimulator";
    QFileSystemModel *model;
    QTimer *timer;
    bool visibleSignals = true; // whether the wires update colours or not
    bool visibleSignalsPreferred = true;
    bool StepMode = false;
    QList<BlockPanel*> blockPanels; // need to keep reference to these to update their appearance
    bool offsetPos;
    lua_State * L;
    static MainWindow * mainWindow;

    // stuff to do with memory component and assembling and stuff
    QVector<int> allowedChars;
    QVector<QByteArray> instructions; // the names of instructions e.g. ADD, LW

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void save();
    void deleteAll(QLayout *layout);

    QList<Component*> getChildrenRecursive(Component* component);
    QString serializeComponents(QList<Component*> toSerialize);
    bool deserializeComponents(QTextStream *in);
    int newComponentId();
    void RemoveComponent(Component *component, bool undoable = true);
    void AddComponent(Component * component, Component *parentComponent);

    void loadUserSettings();
    void saveUserSettings();
    QList<Component *> getSelectedComponents();
    bool anyParentsInList(Component * _component, QList<Component*> _list);
    void resetBlockPanels();
    bool eventFilter(QObject *object, QEvent *event);
    void shouldSignalColoursUpdate();
    void SubStep();

    // lua stuff
    void createComponent(int type, int posX, int posY, int ID, int parentID); // should return component ID
    void changeComponentSettings(int ID, const char *settings);
    void createConnection(const char *outputName, const int inputId, const int outputId, const char *inputName, QList<int> nodes, const int type);

    static int lua_createComponent(lua_State *luaState);
    static int lua_changeComponentSettings(lua_State *luaState);
    static int lua_createConnection(lua_State *luaState);

    Component * deserializeComponent(int type, int posX, int posY, int ID, int parentID);

    void deserializeConnection(QString outputName, int inputId, int outputId, QString inputName, QList<int> nodes, int type);

public slots:
    void Reevaluate();

    void on_AdderToolButton_clicked();

    void on_AndGateToolButton_clicked();

    void on_BlockToolButton_clicked();

    void on_NotGateToolButton_clicked();

    void on_OrGateToolButton_clicked();

    void on_ClockToolButton_clicked();

    void on_action_Open_triggered();

    void on_action_Save_triggered();

    void on_actionSave_As_triggered();

    void on_action_Undo_triggered();

    void on_action_Redo_triggered();

    void on_actionAbout_triggered();

    void on_InputToolButton_clicked();

    void on_OutputToolButton_clicked();

    void on_actionDeselect_triggered();

    void on_SimulationFrequencySpinBox_editingFinished();

    void on_actionDelete_triggered();

    void undoIndexChanged(int index);
private slots:
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_treeView_activated(const QModelIndex &index);
    void on_actionSave_Se_lection_triggered();
    void on_PauseButton_clicked();
    void on_StepButton_clicked();
    void on_MemoryToolButton_clicked();
    void on_NorToolButton_clicked();
    void on_NandToolButton_clicked();
    void on_XorToolButton_clicked();
    void on_CounterToolButton_clicked();
    void on_FlipFlopToolButton_clicked();
    void on_ControlToolButton_clicked();
    void on_MuxToolButton_clicked();
    void on_UpToolButton_clicked();
    void on_DownToolButton_clicked();
    void on_EqualityToolButton_clicked();
    void on_RandomToolButton_clicked();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionUpdate_Wire_Colours_triggered();
    void on_DelayToolButton_clicked();
    void on_DemuxToolButton_clicked();
};

#endif // MAINWINDOW_H
