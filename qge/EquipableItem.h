#pragma once

#include "Vendor.h"

#include "Item.h"

class QPointF;

namespace qge{

class Entity;
class Slot;

/// An abstract class that Represents an Item which is equipable.
class EquipableItem : public Item
{
    // Slot and EquipableItem reference each other. After an EquipableItem has been
    // equipped in a Slot, the slot needs to update the slot_ pointer of the EquipableItem
    friend class Slot;
public:
    EquipableItem();

    QPointF attachmentPoint();
    void setAttachmentPoint(QPointF point);

    bool isEquipped();
    Slot* slotEquippedIn();

private:
    Slot* slotEquippedIn_;
    QPointF attachmentPoint_;
};

}
