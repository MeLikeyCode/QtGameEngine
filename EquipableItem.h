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
public:
    EquipableItem();

    QPointF attachmentPoint();
    void setAttachmentPoint(QPointF point);

    void setLength(double length);
    double length();
    void setWidth(double width);
    double width();

    bool isEquipped();

    bool equip(Slot* slotToEquipIn);
    void unEquip();

protected:
    QPixmap pm_; // allows scaling of the Sprite's QPixmap
    Sprite* spr_;
    QPointF attachmentPoint_;
    bool isEquipped_;
};

#endif // EQUIPABLEITEM_H
