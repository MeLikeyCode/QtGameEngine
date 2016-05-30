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
#include <QPixmap>
#include <QImage>
#include <QGraphicsSceneMouseEvent>

InventoryCell::InventoryCell(int width, int height, Item *item, QGraphicsItem *parent):
    QGraphicsPixmapItem(parent),
    width_(width),
    height_(height),
    item_(item)
{
    // fill background with blue
    QImage img(QSize(width,height),QImage::Format_RGB32);
    img.fill(Qt::darkBlue);
    setPixmap(QPixmap::fromImage(img));

    if (item != nullptr){
        setItem(item);
    }
}

void InventoryCell::setItem(Item *item)
{
    // make sure the item is in an inventory
    assert(item->inventory() != nullptr);

    item_ = item;

    // update graphics
    QPixmap pixmap = item_->sprite()->currentFrame();
    pixmap =  pixmap.scaled(width_,height_);
    setPixmap(pixmap);
}

Item *InventoryCell::item()
{
    return item_;
}

/// Sets the width and height of the InventoryCell.
void InventoryCell::setSize(int width, int height)
{
    width_ = width;
    height_ = height;

    if (item_)
    setPixmap(item_->sprite()->currentFrame().scaled(width,height));
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

    if (event->button() == Qt::RightButton){
        // drop item on ground
        item_->inventory()->removeItem(item_);
        return;
    }

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
        return;
    }
}
