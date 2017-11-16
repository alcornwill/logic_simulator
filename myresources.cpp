#include "myresources.h"
#include <QPixmap>

MyResources::MyResources()
{
    orgate.load(":/Resources/Or.png");
    andgate.load(":/Resources/And.png");
    norgate.load(":/Resources/Nor.png");
    xorgate.load(":/Resources/Xor.png");
    nandgate.load(":/Resources/Nand.png");
    notgate.load(":/Resources/Not.png");
    block.load(":/Resources/Circuit Board.png");
    memory.load(":/Resources/Memory.png");
    input.load(":/Resources/Input.png");
    inputAlt.load(":/Resources/InputAlt.png");
    output.load(":/Resources/Output.png");
    counter.load(":/Resources/Counter.png");
    clock.load(":/Resources/Clock.png");
    mux.load(":/Resources/Mux.png");
    control.load(":/Resources/Control");
    flipflop.load(":/Resources/FlipFlop.png");
    alu.load(":/Resources/Adder.png");
    random.load(":/Resources/Randomizer.png");
    equality.load(":/Resources/Equality.png");
    up.load(":/Resources/UpTransformer2");
    down.load(":/Resources/DownTransformer2");
    delay.load(":/Resources/Delay.png");
    demux.load(":/Resources/Demux.png");

    resizeCursor.setShape(Qt::SizeFDiagCursor);
}
