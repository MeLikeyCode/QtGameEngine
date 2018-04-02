#pragma once

#include "PointTargetItem.h"

namespace qge{

class Sound;

class ItemTeleport : public PointTargetItem
{
public:
    ItemTeleport();

    virtual void use_(const QPointF &point);
private:
    Sound* soundEffect_;
};

}
