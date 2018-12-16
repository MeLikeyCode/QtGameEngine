#include "Inventory.h"

#include "EquipableItem.h"
#include "NoTargetItem.h"
#include "EntityTargetItem.h"
#include "PointTargetItem.h"
#include "Sprite.h"
#include "Entity.h"
#include "Map.h"

using namespace qge;

Inventory::Inventory():
    entity_(nullptr)
{
}

Inventory::~Inventory()
{
    // drop all items on the ground
    for (Item* item: items_){
        item->setInventory(nullptr);
    }
}

/// Returns the Entity that owns this Inventory. Returns nullptr if no Entity
/// owns this inventory.
Entity *Inventory::entity()
{
    return entity_;
}

/// Adds the specified item to the Inventory.
/// If the Item is already in the Inventory, does nothing.
/// Throws if the Item is in another Inventory.
void Inventory::addItem(Item *item)
{
    // if item is already in this inventory, do nothing
    if (contains(item)){
        return;
    }

    // if item is already in another inventory, throw
    Inventory* itemsInventory = item->inventory();
    if (itemsInventory != nullptr && itemsInventory != this){
        assert(false);
    }

    // add to this inventory
    bool stacked = false;
    for (Item* i:items_){ // stack
        if (std::type_index(typeid(*i)) == std::type_index(typeid(*item))){
            i->setNumOfCharges(i->numOfCharges() + item->numOfCharges());
            item->deleteLater();
            stacked = true;
        }
    }
    if (!stacked){ // don't stack
        items_.insert(item);
        item->inventory_ = this;
    }

    // remove item from ground
    Entity* owner = entity();
    if (owner != nullptr){
        Map* theMap = owner->map();
        if (theMap != nullptr){
            theMap->removeEntity(item);
        }
    }

    // listen to when the item dies
    connect(item,&Item::dying,this,&Inventory::onItemDying_);

    emit itemAdded(item);
}

/// Removes the specified Item from the Inventory.
/// Does nothing if the Item is not in the Inventory.
/// If the Inventory belongs to an Entity who is in a map, drops the
/// Item next to that Entity.
void Inventory::removeItem(Item *item)
{
    if (!contains(item))
        return;

    // inventory belongs to entity who is in map
    Entity* owner = entity();
    if (owner != nullptr){
        Map* theMap = owner->map();
        if ( theMap != nullptr){
            QPointF closeToEntity = owner->pos();
            closeToEntity.setY(closeToEntity.y() + 100);
            item->setPos(closeToEntity);
            theMap->addEntity(item);
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

/// Executed when an item of the inventory is dying.
/// Will remove the item from the inventory.
void Inventory::onItemDying_(Entity *dyingItem)
{
    removeItem(static_cast<Item*>(dyingItem));
}
