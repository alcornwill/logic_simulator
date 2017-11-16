#ifndef DEMUX_H
#define DEMUX_H

#include "component.h"

class ByteInputTerminal;
class BitOutputTerminal;

class Demux : public Component
{
    Q_OBJECT
public:
    Demux();

    // bookmarks
    ByteInputTerminal * input;

    virtual void createSettings() override;
    virtual QString SerializeSettings() override;
    virtual void DeserializeSettings(QString string) override;
    virtual void ReevaluateLogic() override;
public slots:
    void addExtraOutput();
    void removeExtraOutput();
};

#endif // DEMUX_H
