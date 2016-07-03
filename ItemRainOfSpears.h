#ifndef ITEMRAINOFSPEARS_H
#define ITEMRAINOFSPEARS_H

#include "NoTargetItem.h"

class QTimer;
class Sound;

class ItemRainOfSpears : public NoTargetItem
{
public:
    ItemRainOfSpears();

    virtual void use();
public slots:
    void spearStep_();
    int times_;
    QTimer* timer_;
    Sound* soundEffect_;
};

#endif // ITEMRAINOFSPEARS_H
