#ifndef MYRESOURCES_H
#define MYRESOURCES_H

// "singleton" to store shared resources like images and specific pens or whatever

#include <QPixmap>
#include <QCursor>

class MyResources
{
public:
    MyResources();

    QPixmap orgate;
    QPixmap andgate;
    QPixmap norgate;
    QPixmap xorgate;
    QPixmap nandgate;
    QPixmap notgate;
    QPixmap block;
    QPixmap memory;
    QPixmap input;
    QPixmap inputAlt;
    QPixmap output;
    QPixmap counter;
    QPixmap clock;
    QPixmap mux;
    QPixmap control;
    QPixmap flipflop;
    QPixmap alu;
    QPixmap random;
    QPixmap equality;
    QPixmap up;
    QPixmap down;
    QPixmap delay;
    QPixmap demux;

    QCursor resizeCursor;

    // should really put pens and stuff in here
};

#endif // MYRESOURCES_H
