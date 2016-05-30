#ifndef EQUIPABLEITEM_H
#define EQUIPABLEITEM_H

#include "Item.h"
#include <QPixmap>

class QPointF;
class Entity;
class Slot;

/// An abstract class that Represents an Item which is equipable.
/// // TODO: make uninstantiateable (abstract)
class EquipableItem : public Item
{
    // Slot and EquipableItem reference each other. After an EquipableItem has been
    // equipped in a Slot, the slot needs to update the slot_ pointer of the EquipableItem
    friend class Slot;
public:
    QPointF attachmentPoint();
    void setAttachmentPoint(QPointF point);

    void setLength(double length);
    double length();
    void setWidth(double width);
    double width();

    bool isEquipped();

protected:
    QPixmap pm_; // allows scaling of the Sprite's QPixmap
    Sprite* spr_;
    QPointF attachmentPoint_;
    Slot* slotEquippedIn_;
};

#endif // EQUIPABLEITEM_H
