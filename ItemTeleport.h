#ifndef ITEMTELEPORT_H
#define ITEMTELEPORT_H

#include "PointTargetItem.h"

class ItemTeleport : public PointTargetItem
{
public:
    ItemTeleport();

    virtual void use(const QPointF &point);
};

#endif // ITEMTELEPORT_H
