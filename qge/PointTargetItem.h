#pragma once

#include "Item.h"

class QPointF;

namespace qge{

class PointTargetAbility;

/// An abstract class that represents an Item that can be used on some location
/// (Point).
///
/// Implementation details
/// ----------------------
/// If the item has an ability assigned (via setAbility() or passed into the ctor)
/// then that ability will be used upon use(), other wise, the virtual use_() function will be
/// called. When the ability of an item is set, its icon/sprite will be changed to the icon of the ability.
/// In other words, by simply giving an item an Ability, you are also giving it an icon and a sprite!
/// @author Abdullah Aghazadah
/// @date 2/13/16
class PointTargetItem : public Item
{
public:
    PointTargetItem(PointTargetAbility* ability=nullptr);

    void use(const QPointF& point);
    void setAbility(PointTargetAbility* ability);
protected:
    virtual void use_(const QPointF& point) = 0;
private:
    PointTargetAbility* ability_;
};

}
