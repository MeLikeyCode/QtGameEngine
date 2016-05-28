#include "Slot.h"
#include "EquipableItem.h"
#include "Sprite.h"
#include "DynamicEntity.h"
#include <cassert>

Slot::Slot():
    item_(nullptr),
    owner_(nullptr)
{
    // empty constructor body
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
    if (item_ != nullptr){
        return true;
    } else {
        return false;
    }
}

/// Equips the specified Item. Returns false if the Item can't be equiped in
/// Slot.
bool Slot::equip(EquipableItem *item)
{
    // make sure the item is in in the owners inventory
    DynamicEntity* o = owner();
    assert(o->inventoryContains(item));

    // return false if the item cannot be equipped
    if (!canBeEquipped(item)){
        return false;
    }

    // =equip item
    // make Item's sprite visible, make sure Item is located in proper location
    item->sprite()->setVisible(true);
    item->setParentEntity(owner());
    item->setPointPos(item->attachmentPoint(),position());
    // update references for both Slot and Item
    item_ = item;
    item->slotEquippedIn_ = this;

    return true;
}

/// Unequips whatever Item is in this Slot.
void Slot::unequip()
{
    // do nothing, if nothing is equiped in the slot
    if (item_ == nullptr){
        return;
    }

    // make the Item invisible
    item_->sprite()->setVisible(false);

    // update references
    item_->slotEquippedIn_ = nullptr;
    item_ = nullptr;
}

/// Returns the EquipableItem housed in the Slot.
EquipableItem *Slot::item()
{
    return item_;
}

/// Returns the Entity that this slot belongs to.
DynamicEntity *Slot::owner()
{
    return owner_;
}
