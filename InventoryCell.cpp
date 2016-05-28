#include "InventoryCell.h"
#include "Item.h"
#include "Axe.h"
#include <QDebug>
#include <QBrush>
#include "NoTargetItem.h"
#include "DynamicEntity.h"
#include "Inventory.h"
#include "Slot.h"
#include "EquipableItem.h"
#include <cassert>

InventoryCell::InventoryCell(QGraphicsItem *parent):
    QGraphicsRectItem(parent)
{
    setRect(0,0,64,64);

    QBrush brush;
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::SolidPattern);
    setBrush(brush);
}

void InventoryCell::setItem(Item *item)
{
    // make sure the item is in an inventory
    assert(item->inventory() != nullptr);

    // update graphics
    QBrush brush;
    QPixmap texture = item->sprite()->currentFrame().scaledToHeight(rect().height());
    texture = texture.scaledToWidth(rect().width());
    brush.setTexture(texture);
    setBrush(brush);

    item_ = item;
}

Item *InventoryCell::item()
{
    return item_;
}

void InventoryCell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // logic:
    // if item type is EquipableItem:
    // - find out the specific type of the equipable item and see if inventory->entity has a slot for it
    // - if so:
    //  - unequip item in that slot, place in inv cell, equip clicked item in slot
    // if item type is NoTargetItem -> call its use()
    // if item type is PointTargetItem, set games mouse mode to select position mode, set cursor, etc...
    //  -have a PositionSelectedWhileInPointTargetMode, that will then use(position)
    // if item type is EntityTargetItem, similar

    // no item TODO: return

    // NoTargetItem
    NoTargetItem* asNoTargetItem = dynamic_cast<NoTargetItem*>(item());
    if (asNoTargetItem){
        asNoTargetItem->use();
        return;
    }

    // EquipableItem
    // traverse through all slots of entity looking for a slot that can equip this type of EquipableItem
    // if there is already an item in that slot, swap em
    EquipableItem* asEquipableItem = dynamic_cast<EquipableItem*>(item());
    DynamicEntity* owner = item()->inventory()->entity();
    Slot* firstViableSlot = nullptr;
    for (Slot* slot:owner->getSlots()){
        if (slot->canBeEquipped(asEquipableItem)){
            firstViableSlot = slot;
            break;
        }
    }
    if (firstViableSlot){
        // if there is already an item in that slot, take it out
        if (firstViableSlot->isFilled()){
            firstViableSlot->unequip();
        }
        firstViableSlot->equip(asEquipableItem);
    }
}
