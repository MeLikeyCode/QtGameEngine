#include "EquipableItem.h"
#include <QPointF>
#include <cassert>
#include "Inventory.h"
#include "Slot.h"

EquipableItem::EquipableItem()
{
    
}

// Returns true if the EquipableItem is equipped, otherwise false.
bool EquipableItem::isEquipped()
{
    return isEquipped_;
}

/// Equips the EquipableItem at the specified Slot.
/// Returns true if equipped sucessfully, otherwise returns false (if the Slot
/// can't equip the Item).
bool EquipableItem::equip(Slot *slotToEquipIn)
{
    // make sure the Item is not on the ground
    // only items in the inventory can be equipped
    assert(inventory() != nullptr);

    // equp, if it can be equipped
    if (slotToEquipIn->canBeEquipped(this)){

        sprite()->setVisible(true);
        sprite()->setScale(1);
        setParentEntity(inventory()->entity());
        setPointPos(attachmentPoint(),slotToEquipIn->position());

        slotToEquipIn->item_ = this;
        slotToEquipIn->filled_ = true;
        return true;
    }
    else {
        return false;
    }

}

/// Unequips the EquipableItem. Does nothing if its already unequiped.
void EquipableItem::unEquip()
{
    if (isEquipped()){
        sprite()->setVisible(false);
    }
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
