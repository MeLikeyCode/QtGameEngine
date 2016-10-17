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
#include "PointTargetItem.h"
#include "EntityTargetItem.h"
#include <cassert>
#include <QPixmap>
#include <QImage>
#include <QGraphicsSceneMouseEvent>
#include "Game.h"
#include "Panel.h"

InventoryCell::InventoryCell(Game* game, int width, int height, Item *item):
    background_(new Panel()),
    item_(item),
    game_(game),
    picture_(new QGraphicsPixmapItem())
{
    picture_->setParentItem(background_->getGraphicsItem());
    background_->setWidth(width);
    background_->setHeight(height);
    connect(background_,&Panel::clicked,this,&InventoryCell::onClicked_);
    draw_();
}

void InventoryCell::setItem(Item *item)
{
    // make sure the item is in an inventory
    assert(item->inventory() != nullptr);

    item_ = item;
    draw_();
}

Item *InventoryCell::item()
{
    return item_;
}

/// Sets the width and height of the InventoryCell.
void InventoryCell::setSize(int width, int height)
{
    background_->setWidth(width);
    background_->setHeight(height);
}

/// Sets the background color of the InventoryCell.
/// Remember that opacity can be included in the color information.
/// If this function is called last, the background of the InventoryCell will
/// be a color, however, if setBackgroundPixmap(const QPixmap&) is called last,
/// the background will be a pixmap.
void InventoryCell::setBackgroundColor(const QColor &color)
{
    background_->setBackgroundColor(color);
}

/// Sets the background of the InventoryCell to be a QPixmap.
/// @see setBackgroundColor(const QColor&) for more info.
void InventoryCell::setBackgroundPixmap(const QPixmap &pixmap)
{
    background_->setBackgroundPixmap(pixmap);
}

QGraphicsItem *InventoryCell::getGraphicsItem()
{
    return background_->getGraphicsItem();
}

/// Executed when the InventoryCell is clicked. Will "use" the item clicked.
/// See internal comment for more info.
void InventoryCell::onClicked_(Panel* panel, QPointF pos, int button)
{
    // logic:
    // if item is an EquipableItem:
    // - find out the specific type of the equipable item and see if inventory->entity has a slot for it
    //      - if so:
    //          - unequip item in that slot, place it in inv cell, equip clicked item in slot
    // if item type is NoTargetItem -> call its use()
    // if item type is PointTargetItem, set games mouse mode to select position mode, set cursor, etc...
    //  -have a PositionSelectedWhileInPointTargetMode, that will then use(position)
    // if item type is EntityTargetItem, similar

    Item* theItem = item();

    // if there is no item in the InventoryCell, do nothing
    if (theItem == nullptr){
        return;
    }

    // if right mouse button, drop the item clicked on the ground
    if (button == Qt::RightButton){
        // if its an equipeable item that is equiped, unequip it
        EquipableItem* asEqupiableItem = dynamic_cast<EquipableItem*>(theItem);
        if (asEqupiableItem){
            if (asEqupiableItem->isEquipped()){
                asEqupiableItem->slotEquippedIn()->unequip();
            }
        }
        // drop item on ground
        theItem->inventory()->removeItem(theItem);
        return;
    }

    // if left mouse button, do something depending on the type of the Item
    if (button == Qt::LeftButton){

        // NoTargetItem, use it
        NoTargetItem* asNoTargetItem = dynamic_cast<NoTargetItem*>(theItem);
        if (asNoTargetItem){
            asNoTargetItem->use();
            return;
        }

        // EquipableItem
        // traverse through all slots of entity looking for a slot that can equip this type of EquipableItem
        // if there is already an item in that slot, swap em
        EquipableItem* asEquipableItem = dynamic_cast<EquipableItem*>(theItem);
        DynamicEntity* owner = theItem->inventory()->entity();
        Slot* viableSlot = nullptr;
        for (Slot* slot:owner->getSlots()){
            if (slot->canBeEquipped(asEquipableItem)){
                viableSlot = slot;
                break;
            }
        }
        if (viableSlot){
            // if there is already an item in that slot, take it out
            if (viableSlot->isFilled()){
                viableSlot->unequip();
            }
            viableSlot->equip(asEquipableItem);
            return;
        }

        // PointTargetItem
        // Put mouseMode of the game to select position, connect to mouseMode's
        // positionSelected
        PointTargetItem* asPointTargetItem = dynamic_cast<PointTargetItem*>(theItem);
        if (asPointTargetItem){
            game_->setMouseMode(Game::MouseMode::selectPosition);
            QObject::disconnect(game_,&Game::positionSelected,this,&InventoryCell::onPositionSelectedWhileUsingPointTargetItem);
            QObject::connect(game_,&Game::positionSelected,this,&InventoryCell::onPositionSelectedWhileUsingPointTargetItem);
            return;
        }


        // EntityTargetItem
        // Similar as above, except entitySelected mode instead of positionSelected mode
        EntityTargetItem* asEntityTargetItem = dynamic_cast<EntityTargetItem*>(theItem);
        if (asEntityTargetItem){
            game_->setMouseMode(Game::MouseMode::selectEntity);
            QObject::disconnect(game_,&Game::entitySelected,this,&InventoryCell::onEntitySelectedWhileUsingEntityTargetItem);
            QObject::connect(game_,&Game::entitySelected,this,&InventoryCell::onEntitySelectedWhileUsingEntityTargetItem);
            return;
        }
    }
}

/// Executed when a position has been selected, while using a PointTargetItem.
void InventoryCell::onPositionSelectedWhileUsingPointTargetItem(QPointF pos)
{
    // make sure the Item is PointTargetItem
    PointTargetItem* asPointTargetItem = dynamic_cast<PointTargetItem*>(item());
    assert (asPointTargetItem != nullptr);

    // use the item at the position
    asPointTargetItem->use(pos);

    // disconnect
    QObject::disconnect(game_,&Game::positionSelected,this,&InventoryCell::onPositionSelectedWhileUsingPointTargetItem);

    // set regular mouse mode
    game_->setMouseMode(Game::MouseMode::regular);
}

/// Executed when an entity has been selected, while using a EntitySelectItem.
void InventoryCell::onEntitySelectedWhileUsingEntityTargetItem(Entity *ent)
{
    // use the item at the entity
    EntityTargetItem* asEntityTargetItem = dynamic_cast<EntityTargetItem*>(item());
    asEntityTargetItem->use(ent);

    // disconnect
    QObject::disconnect(game_,&Game::entitySelected,this,&InventoryCell::onEntitySelectedWhileUsingEntityTargetItem);

    // set regular mouse mode
    game_->setMouseMode(Game::MouseMode::regular);
}

void InventoryCell::draw_()
{
    // draw item's picture
    if (item_ != nullptr){
        double width = background_->width();
        double height = background_->height();
        picture_->setPixmap(item_->sprite()->currentFrame().scaled(width-20,height-20));
        picture_->setPos(10,10);
    }
}
