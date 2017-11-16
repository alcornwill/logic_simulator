#include "terminal.h"

#ifndef INPUTTERMINAL_H
#define INPUTTERMINAL_H

class OutputTerminal;
class Component;
class Connection;

class InputTerminal : public Terminal
{
public:
    InputTerminal();

    virtual OutputTerminal * getOutputTerminal() = 0;
    virtual Connection * getConnection() = 0;

    bool connected = false; // this value is manipulated by derivatives
    bool hoverState = false;

    void updatePosition();
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
};

#endif // INPUTTERMINAL_H
