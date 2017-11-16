#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T12:13:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogicSimulator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    terminal.cpp \
    component.cpp \
    andgate.cpp \
    orgate.cpp \
    inputterminal.cpp \
    outputterminal.cpp \
    mygraphicsscene.cpp \
    maincontext.cpp \
    connection.cpp \
    movecommand.cpp \
    deletecommand.cpp \
    createcommand.cpp \
    createconnectioncommand.cpp \
    deleteconnectioncommand.cpp \
    blockcomponent.cpp \
    blocknodecomponent.cpp \
    blockpanel.cpp \
    notgate.cpp \
    logicgate.cpp \
    bitinputterminal.cpp \
    bitoutputterminal.cpp \
    bitconnection.cpp \
    byteinputterminal.cpp \
    byteoutputterminal.cpp \
    byteconnection.cpp \
    bitblocknodecomponent.cpp \
    byteblocknodecomponent.cpp \
    inputcomponent.cpp \
    outputcomponent.cpp \
    memorycomponent.cpp \
    upcomponent.cpp \
    downcomponent.cpp \
    flipflopcomponent.cpp \
    muxcomponent.cpp \
    xorcomponent.cpp \
    nandcomponent.cpp \
    norcomponent.cpp \
    controlcomponent.cpp \
    countercomponent.cpp \
    clockcomponent.cpp \
    randomcomponent.cpp \
    equalitycomponent.cpp \
    myresources.cpp \
    connectionnode.cpp \
    alucomponent.cpp \
    delaycomponent.cpp \
    demux.cpp

HEADERS  += mainwindow.h \
    terminal.h \
    component.h \
    andgate.h \
    orgate.h \
    inputterminal.h \
    outputterminal.h \
    mygraphicsscene.h \
    maincontext.h \
    connection.h \
    movecommand.h \
    deletecommand.h \
    createcommand.h \
    createconnectioncommand.h \
    deleteconnectioncommand.h \
    blockcomponent.h \
    blocknodecomponent.h \
    blockpanel.h \
    notgate.h \
    logicgate.h \
    connectiontype.h \
    bitinputterminal.h \
    bitoutputterminal.h \
    bitconnection.h \
    byteinputterminal.h \
    byteoutputterminal.h \
    byteconnection.h \
    bitblocknodecomponent.h \
    byteblocknodecomponent.h \
    inputcomponent.h \
    outputcomponent.h \
    memorycomponent.h \
    upcomponent.h \
    downcomponent.h \
    flipflopcomponent.h \
    muxcomponent.h \
    xorcomponent.h \
    nandcomponent.h \
    norcomponent.h \
    controlcomponent.h \
    countercomponent.h \
    clockcomponent.h \
    randomcomponent.h \
    equalitycomponent.h \
    myresources.h \
    connectionnode.h \
    alucomponent.h \
    delaycomponent.h \
    demux.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

LIBS += liblua53.a

