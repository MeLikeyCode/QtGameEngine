#include "Inventory.h"
#include "EquipableItem.h"
#include "NoTargetItem.h"
#include "EntityTargetItem.h"
#include "PointTargetItem.h"
#include <cassert>

Inventory::Inventory()
{
    // initialize
    entity_ = nullptr;
}

/// Returns the Entity that owns this Inventory.
Entity *Inventory::entity()
{
    return entity_;
}

/// Set the Entity that this Inventory belongs to.
void Inventory::setEntity(Entity *entity)
{
    entity_ = entity;
}

/// Adds the specified item to the Inventory.
void Inventory::addItem(Item *item)
{
    items_.insert(item);
    item->setInventory(this);
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
