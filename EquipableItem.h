#ifndef EQUIPABLEITEM_H
#define EQUIPABLEITEM_H

#include "Item.h"
#include <QPixmap>

class QPointF;
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
    QPointF attachmentPoint_;
    Slot* slotEquippedIn_;
};

#endif // EQUIPABLEITEM_H
