#include "EquipableItem.h"

#include "Inventory.h"
#include "Slot.h"
#include "Entity.h"

using namespace qge;

EquipableItem::EquipableItem():
    slotEquippedIn_(nullptr),
    attachmentPoint_()
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
