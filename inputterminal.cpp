#include "inputterminal.h"
#include "outputterminal.h"
#include "connection.h"

InputTerminal::InputTerminal()
{
    setAcceptHoverEvents(true);
}

void InputTerminal::updatePosition()
{
    // update wire position
    if (connected) getConnection()->updatePosition();
}


void InputTerminal::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (connected)
    {
        getConnection()->hoverState = true;
        getConnection()->updateColor();
        // also do this, it's a bit messy but whatever
        Connection::highlighted = getConnection();
    }

}

void InputTerminal::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (connected)
    {
        getConnection()->hoverState = false;
        getConnection()->updateColor();

        Connection::highlighted = 0;
    }
}
