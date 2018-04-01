#pragma once

#include "Item.h"

class QPointF;

namespace qge{

/// An abstract class that represents an Item that can be used on some location
/// (Point).
/// @author Abdullah Aghazadah
/// @date 2/13/16
class PointTargetItem : public Item
{
public:
    void use(const QPointF& point);
protected:
    virtual void use_(const QPointF& point) = 0;
};

}
