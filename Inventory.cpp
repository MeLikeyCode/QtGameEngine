#include "Inventory.h"
#include "EquipableItem.h"
#include "NoTargetItem.h"
#include "EntityTargetItem.h"
#include "PointTargetItem.h"
#include <cassert>
#include "Sprite.h"
#include "DynamicEntity.h"
#include "Map.h"

Inventory::Inventory():
    entity_(nullptr)
{
}

/// Returns the Entity that owns this Inventory. Returns nullptr if no Entity
/// owns this inventory.
DynamicEntity *Inventory::entity()
{
    return entity_;
}

/// Adds the specified item to the Inventory.
/// If the Item is already in the Inventory, does nothing.
/// If the Item is in another Inventory, it will be removed from there first.
void Inventory::addItem(Item *item)
{
    // if item is already in this inventory, do nothing
    if (contains(item)){
        return;
    }

    // if item is in another inventory, remove it first
    Inventory* itemsInventory = item->inventory();
    if (itemsInventory != nullptr && itemsInventory != this){
        itemsInventory->removeItem(item);
    }

    // add to this inventory
    items_.insert(item);
    item->inventory_ = this;

    // make sprite of item invisible (no longer on ground)
    item->sprite()->setVisible(false);

    item->setInvulnerable(true);

    // if the inventory belongs to an entity that is on a map, add item to map
    DynamicEntity* owner = entity();
    if (owner != nullptr){
        Map* theMap = owner->map();
        if (theMap != nullptr){
            theMap->addEntity(item);
        }
    }

    emit itemAdded(item);
}

/// Removes the specified Item from the Inventory.
/// If the Item is not in the Inventory, does nothing.
/// If the Inventory belongs to an Entity who is in a map, drops the
/// Item next to that Entity.
void Inventory::removeItem(Item *item)
{
    // item not in inventory
    if (!contains(item)){
        return;
    }

    // inventory belongs to entity who is in map
    DynamicEntity* owner = entity();
    if (owner != nullptr){
        Map* theMap = owner->map();
        if ( theMap != nullptr){
            theMap->addEntity(item);
            item->sprite()->setVisible(true);
            item->setInvulnerable(false);
            QPointF closeToEntity = owner->pointPos();
            closeToEntity.setY(closeToEntity.y() + 100);
            item->setPointPos(closeToEntity);
        }
    }

    items_.erase(item);
    item->inventory_ = nullptr;
    emit itemRemoved(item);
}

/// Returns true if the Inventory contains the specified item.
bool Inventory::contains(Item *item)
{
    return items_.count(item) != 0;
}

/// Returns all the EquipableItems in the Inventory.
std::unordered_set<EquipableItem *> Inventory::getEquipableItems()
{
    // traverse through items_, if item is EquipableItem, add it to equipableItems
    std::unordered_set<EquipableItem*> equipableItems;
    for (Item* item:items_){
        EquipableItem* toEquipableItem = dynamic_cast<EquipableItem*>(item);
        if (toEquipableItem != nullptr){
            equipableItems.insert(toEquipableItem);
        }
    }
    return equipableItems;
}

/// Returns all the NoTargetItems in the Inventory.
std::unordered_set<NoTargetItem *> Inventory::getNoTargetItems()
{
    // traverse through items_, if item is NoTargetITem, add it to noTargetItems
    std::unordered_set<NoTargetItem*> noTargetItems;
    for (Item* item:items_){
        NoTargetItem* toNoTargetItem = dynamic_cast<NoTargetItem*>(item);
        if (toNoTargetItem != nullptr){
            noTargetItems.insert(toNoTargetItem);
        }
    }
    return noTargetItems;
}

/// Returns all the EntityTargetItems in the Inventory.
std::unordered_set<EntityTargetItem *> Inventory::getEntityTargetItems()
{
    // traverse through items_, if item is EntityTargetItem, add it to entityTargetItems
    std::unordered_set<EntityTargetItem*> entityTargetItems;
    for (Item* item:items_){
        EntityTargetItem* toEntityTargetItem = dynamic_cast<EntityTargetItem*>(item);
        if (toEntityTargetItem != nullptr){
            entityTargetItems.insert(toEntityTargetItem);
        }
    }
    return entityTargetItems;
}

/// Returns all the PointTargetItems in the Inventory.
std::unordered_set<PointTargetItem *> Inventory::getPointTargetItems()
{
    // traverse through items_, if item is PointTargetItem, add it to pointTargetItems
    std::unordered_set<PointTargetItem*> pointTargetItems;
    for (Item* item:items_){
        PointTargetItem* toPointTargetItem = dynamic_cast<PointTargetItem*>(item);
        if (toPointTargetItem != nullptr){
            pointTargetItems.insert(toPointTargetItem);
        }
    }
    return pointTargetItems;
}

/// Returns all the Items in the Inventory.
std::unordered_set<Item *> Inventory::getItems()
{
    return items_;
}
