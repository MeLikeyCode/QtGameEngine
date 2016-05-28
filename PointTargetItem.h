#ifndef POINTTARGETENTITY_H
#define POINTTARGETENTITY_H

#include "Item.h"

class QPointF;

/// An abstract class that represents an Item that can be used on some location
/// (Point).
/// @author Abdullah Aghazadah
/// @date 2/13/16
class PointTargetItem : public Item
{
public:
    virtual void use(const QPointF& point) = 0;
};

#endif // POINTTARGETENTITY_H
