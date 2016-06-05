#include "EquipableItem.h"
#include <QPointF>
#include <cassert>
#include "Inventory.h"
#include "Slot.h"
#include "DynamicEntity.h"

EquipableItem::EquipableItem():
    slotEquippedIn_(nullptr)
{

}

// Returns true if the EquipableItem is equipped, otherwise false.
bool EquipableItem::isEquipped()
{
    if (slotEquippedIn_ != nullptr){
        return true;
    } else {
        return false;
    }
}

/// Returns the Slot that the EquipableItem is equipped in.
/// Returns nullptr if not equipped in any slot.
Slot *EquipableItem::slotEquippedIn()
{
    return slotEquippedIn_;
}

/// Returns the EquipableItem's attachment point. The attachment point is the
/// position of the EquipableItem that is to be attached to the Entity.
QPointF EquipableItem::attachmentPoint()
{
    return attachmentPoint_;
}

/// Set the attachment point of the EquipableItem.
/// @see EquipableItem::attachmentPoint()
void EquipableItem::setAttachmentPoint(QPointF point)
{
    attachmentPoint_ = point;
}

/// Sets the length of the EquipableItem in pixels.
void EquipableItem::setLength(double length)
{
    pm_ = pm_.scaled(length,pm_.height());
    spr_->setPixmap(pm_);
}

/// Returns the length of the EquipableItem in pixels.
double EquipableItem::length()
{
    return sprite()->boundingRect().width();
}

/// Sets the width of the EquipableItem in pixels.
void EquipableItem::setWidth(double width)
{
    pm_ = pm_.scaled(pm_.width(),width);
    spr_->setPixmap(pm_);
}

/// Returns the width of the EquipableItem in pixels.
double EquipableItem::width()
{
    return sprite()->boundingRect().height();
}
