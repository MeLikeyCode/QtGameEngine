#include "Slot.h"

#include "EquipableItem.h"
#include "Sprite.h"
#include "Entity.h"
#include "Map.h"
#include "Inventory.h"

using namespace qge;

Slot::Slot():
    item_(nullptr),
    owner_(nullptr)
{
    // empty constructor body
}

/// Sets the position of the Slot.
/// The position determines where any EquipableItems equipped in the Slot appear
/// relative to the owning Entity.
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
    Entity* theOwner = owner();
    assert(theOwner != nullptr);    // make sure has an owner

    // if item is not in inventory, add it
    auto inv = theOwner->inventory();
    if (!inv->contains(item))
        inv->addItem(item);

    // return false if the item cannot be equipped
    if (!canBeEquipped(item)){
        return false;
    }

    // equip item
    if (theOwner->map() != nullptr)
        theOwner->map()->addEntity(item);  // make sure the item is in the owners map
    item->setParentEntity(theOwner);
    item->setPos(item->attachmentPoint(),position());

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

    // remove the item from the map (will be added back when its equipped)
    item_->setParentEntity(nullptr);
    item_->map()->removeEntity(item_);

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
Entity *Slot::owner()
{
    return owner_;
}
