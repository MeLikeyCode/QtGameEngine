#include "Slot.h"
#include "EquipableItem.h"
#include "Sprite.h"
#include "DynamicEntity.h"
#include <cassert>
#include "Map.h"

Slot::Slot():
    item_(nullptr),
    owner_(nullptr)
{
    // empty constructor body
}

/// Sets the position of the Slot.
/// The position determines where any EquipableItems equipped in the Slot appear
/// relative to the owning DynamicEntity.
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
    return item_ != nullptr;
}

/// Attempts to equip the specified item. Returns true if the Item was
/// succesfully equipped, otherwise false.
bool Slot::equip(EquipableItem *item)
{
    // assertions
    DynamicEntity* theOwner = owner();
    assert(theOwner != nullptr);    // make sure has an owner
    assert(theOwner->inventoryContains(item));  // make sure owner's inventory contains
                                                // the item being equipped in this slot

    // return false if the item cannot be equipped
    if (!canBeEquipped(item)){
        return false;
    }

    // =equip item
    // make Item's sprite visible, make sure Item is located in proper location
    if (theOwner->map() != nullptr)
        theOwner->map()->addEntity(item);  // make sure the item is in the owners map
    item->setParentEntity(theOwner);
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

    // make the Item invisible/in proper place
    item_->setParentEntity(nullptr);

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
