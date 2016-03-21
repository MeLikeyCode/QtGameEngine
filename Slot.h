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
    // EquipableItem and Slot reference each other
    // The association behavior is only in one of them (EquipableItem),
    // the others setter simply delegates.
    // (same with entity)
    friend class EquipableItem;
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
    QPointF position_; // pos that the item should go in
    std::string name_; // (relative to the entity the slot is in)
    bool filled_;
    EquipableItem* item_; // item this slot houses
    DynamicEntity* owner_; // who this slot belongs to
};

#endif // SLOT_H
