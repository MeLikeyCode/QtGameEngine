#pragma once

#include "Vendor.h"

namespace qge{

class EquipableItem;
class Entity;

/// An abstract class that represents an equipment slot.
/// Entities can have Inventories and Slots. An entity can equip items that are in its inventory to its slots.
/// There are different types of slots, each of which can equip different types of items.
/// For example, MeleeWeapon slots can only equip MeleeWeapons.
/// @author Abdullah Aghazadah
/// @date 2/17/16
class Slot
{
    // Entity has an addSlot(Slot* s) function, that needs to update the Slot's owner_
    // attribute
    friend class Entity;
public:
    Slot();

    void setPosition(const QPointF& pos);
    QPointF position();

    void setName(const std::string& s);
    std::string name();

    bool isFilled();

    bool equip(EquipableItem* item);
    void unequip();

    EquipableItem* item();

    Entity* owner();

    /// Returns true if the specified Item can be equipped in this Slot.
    virtual bool canBeEquipped(EquipableItem* item) = 0;

    /// Use the item in the Slot.
    /// Suggested implementation: do something with the Item in the Slot.
    virtual void use() = 0;

protected:
    QPointF position_; // pos (relative to Entity that owns the Slot) that the item should go in
    std::string name_; // a string name for the slot, i.e. "rightHand", "chest", etc...
    EquipableItem* item_; // item this slot houses
    Entity* owner_; // who this slot belongs to
};

}
