#ifndef SLOT_H
#define SLOT_H

#include <QPointF>
#include <string>

class EquipableItem;

/// An abstract class that represents an equipment slot.
/// @author Abdullah Aghazadah
/// @date 2/17/16
class Slot
{
public:
    Slot();

    void setPosition(const QPointF& pos);
    QPointF position();

    void setName(const std::string& s);
    std::string name();

    bool isFilled();

    bool equip(EquipableItem* item);
    void unequip();

    /// Returns true if the specified Item can be equipped in this Slot.
    virtual bool canBeEquipped(EquipableItem* item) = 0;

    /// Use the item in the Slot.
    /// Suggested implementation: do something with the Item in the Slot.
    virtual void use() = 0;

protected:
    QPointF position_;
    std::string name_;
    bool filled_;
    EquipableItem* item_;
};

#endif // SLOT_H
