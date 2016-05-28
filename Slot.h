#ifndef SLOT_H
#define SLOT_H

#include <QPointF>
#include <string>

class EquipableItem;
class DynamicEntity;

/// An abstract class that represents an equipment slot.
/// @author Abdullah Aghazadah
/// @date 2/17/16
class Slot
{
    // Entity has an addSlot(Slot* s) function, that needs to update the Slot's owner_
    // attribute
    friend class DynamicEntity;
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

    DynamicEntity* owner();

    /// Returns true if the specified Item can be equipped in this Slot.
    virtual bool canBeEquipped(EquipableItem* item) = 0;

    /// Use the item in the Slot.
    /// Suggested implementation: do something with the Item in the Slot.
    virtual void use() = 0;

protected:
    QPointF position_; // pos (relative to Entity that owns the Slot) that the item should go in
    std::string name_; // a string name for the slot, i.e. "rightHand", "chest", etc...
    EquipableItem* item_; // item this slot houses
    DynamicEntity* owner_; // who this slot belongs to
};

#endif // SLOT_H
