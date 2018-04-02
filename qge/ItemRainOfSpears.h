#pragma once

#include "NoTargetItem.h"

class QTimer;

namespace qge{

class Sound;

class ItemRainOfSpears : public NoTargetItem
{
public:
    ItemRainOfSpears();

    virtual void use_();
public slots:
    void spearStep_();
    int times_;
    QTimer* timer_;
    Sound* soundEffect_;
};

}
