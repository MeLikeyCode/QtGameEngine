#include "InventoryUser.h"

#include "InventoryViewer.h"
#include "Game.h"
#include "EquipableItem.h"
#include "Slot.h"
#include "Inventory.h"
#include "NoTargetItem.h"
#include "PointTargetItem.h"
#include "EntityTargetItem.h"
#include "Entity.h"

using namespace qge;

/// Constructs an InventoryViewer.
/// Game is needed because "using" some items requires changing the
/// Game's mouse mode.
InventoryUser::InventoryUser(Game *game, Inventory *inventory):
    inventoryViewer_(new InventoryViewer(inventory)),
    game_(game),
    lastItemUsed_(nullptr)
{
    // listen to InventoryViewer click signals
    connect(inventoryViewer_.get(),&InventoryViewer::itemClicked,this,&InventoryUser::onItemClicked);
}

QGraphicsItem *InventoryUser::getGraphicsItem()
{
    return inventoryViewer_->getGraphicsItem();
}

/// Executed when an Item of the InventoryUser has been clicked.
/// Will "use" the item if left clicked.
/// Will drop the item (at location of item's inventory's owner) if right clicked.
/// "use" depends on the type of the Item.
/// If the Item is an EquipableItem, an attempt will be made to equip it into
/// a viable slot. If no viable Slot is found, nothing happens. If a viable Slot
/// is found but its taken by another Item, that Item will be unequiped, and the
/// clicked Item will be equipped in its place.
/// If the Item is a NoTargetItem, its NoTargetItem::use() function will be called.
/// If the Item is a PointTargetItem, the Game's mouse mode will be set to selectPosition,
/// once a position is selected, the item will be used at that position.
/// If the Item is an EntityTargetItem, the Game's mouse mode will be set to selectEntity,
/// once an Entity is selected, the item will be used on that Entity.
/// If an Item is right clicked, it will be dropped from the Inventory (at the position
/// of the entity that owns the inventory) regardless of the type of the Item.
void InventoryUser::onItemClicked(Item *item, int button)
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

        lastItemUsed_ = item;
        Item* theItem = item;

        // if no item was clicked, do nothing
        // TODO: smells bad, why is this callback even called with a null item?
        // (I don't think it is, remove this and see if everything works)
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
            Entity* owner = theItem->inventory()->entity();
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
                QObject::disconnect(game_,&Game::positionSelected,this,&InventoryUser::onPositionSelectedWhileUsingPointTargetItem);
                QObject::connect(game_,&Game::positionSelected,this,&InventoryUser::onPositionSelectedWhileUsingPointTargetItem);
                return;
            }


            // EntityTargetItem
            // Similar as above, except entitySelected mode instead of positionSelected mode
            EntityTargetItem* asEntityTargetItem = dynamic_cast<EntityTargetItem*>(theItem);
            if (asEntityTargetItem){
                game_->setMouseMode(Game::MouseMode::selectEntity);
                QObject::disconnect(game_,&Game::entitySelected,this,&InventoryUser::onEntitySelectedWhileUsingEntityTargetItem);
                QObject::connect(game_,&Game::entitySelected,this,&InventoryUser::onEntitySelectedWhileUsingEntityTargetItem);
                return;
            }
        }
}

/// Executed when a position has been selected, while using a PointTargetItem.
void InventoryUser::onPositionSelectedWhileUsingPointTargetItem(QPointF pos)
{
    assert(lastItemUsed_ != nullptr);

    // make sure the Item is PointTargetItem
    PointTargetItem* asPointTargetItem = dynamic_cast<PointTargetItem*>(lastItemUsed_);
    assert (asPointTargetItem != nullptr);

    // use the item at the position
    asPointTargetItem->use(pos);

    // disconnect
    QObject::disconnect(game_,&Game::positionSelected,this,&InventoryUser::onPositionSelectedWhileUsingPointTargetItem);

    // set regular mouse mode
    game_->setMouseMode(Game::MouseMode::regular);
}

void InventoryUser::onEntitySelectedWhileUsingEntityTargetItem(Entity *entity)
{
    assert(lastItemUsed_ != nullptr);

    // use the item at the entity
    EntityTargetItem* asEntityTargetItem = dynamic_cast<EntityTargetItem*>(lastItemUsed_);
    asEntityTargetItem->use(entity);

    // disconnect
    QObject::disconnect(game_,&Game::entitySelected,this,&InventoryUser::onEntitySelectedWhileUsingEntityTargetItem);

    // set regular mouse mode
    game_->setMouseMode(Game::MouseMode::regular);
}
