#ifndef ITEMTELEPORT_H
#define ITEMTELEPORT_H

#include "PointTargetItem.h"

class Sound;

class ItemTeleport : public PointTargetItem
{
public:
    ItemTeleport();

    virtual void use_(const QPointF &point);
private:
    Sound* soundEffect_;
};

#endif // ITEMTELEPORT_H
