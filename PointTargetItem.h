#ifndef POINTTARGETENTITY_H
#define POINTTARGETENTITY_H

#include "Item.h"

class QPointF;

/// Represents an Item that can be used on some location (Point).
/// @author Abdullah Aghazadah
/// @date 2/13/16
class PointTargetItem : public Item
{
public:
    PointTargetItem();
    void use(const QPointF& point);
};

#endif // POINTTARGETENTITY_H
