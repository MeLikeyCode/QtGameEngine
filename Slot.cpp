#include "Slot.h"

Slot::Slot()
{
}

/// Sets the position of the Slot.
/// The position determines where any Items equipped in the Slot appear.
void Slot::setPosition(const QPointF &pos)
{
    position_ = pos;
}

/// Returns the position of the Slot.
/// @see Slot::setPosition(const QPointF&)
QPointF Slot::position()
{
    return position_;
}

/// Sets the name of the Slot.
/// Every Slot should have a name (e.g. "leftHand", "head", "legs", etc...).
void Slot::setName(const std::string &s)
{
    name_ = s;
}

/// Returns the name of the Slot.
/// @see Slot::setName(const std::string&)
std::string Slot::name()
{
    return name_;
}

/// Returns true if the Slot is filled (i.e. taken by an Item).
bool Slot::isFilled()
{
    return filled_;
}

/// Equips the specified Item. Returns false if the Item can't be equiped in
/// Slot.
bool Slot::equip(EquipableItem *item)
{
    if (canBeEquipped(item)){
        item_ = item;
        filled_ = true;
        return true;
    }
    else{
        return false;
    }
}

/// Unequips whatever Item is in this Slot.
void Slot::unequip()
{
    item_ = nullptr;
    filled_ = false;
}
