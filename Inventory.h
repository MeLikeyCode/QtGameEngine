#ifndef INVENTORY_H
#define INVENTORY_H

#include <unordered_set>

class Item;
class Entity;
class EquipableItem;
class NoTargetItem;
class EntityTargetItem;
class PointTargetItem;

/// Represents a collection of Items that can belong to an Entity.
class Inventory
{
    // DynamicEntity and Inventory reference each other.
    // The association code is in the Entity, which needs private access
    // to set the pointer in Inventory.
    friend class DynamicEntity;
public:
    Inventory();

    Entity* entity();

    void addItem(Item* item);
    void removeItem(Item* item);
    bool contains(Item* item);

    std::unordered_set<EquipableItem*> getEquipableItems();
    std::unordered_set<NoTargetItem*> getNoTargetItems();
    std::unordered_set<EntityTargetItem*> getEntityTargetItems();
    std::unordered_set<PointTargetItem*> getPointTargetItems();
    std::unordered_set<Item*> getItems();

private:
    Entity* entity_;
    std::unordered_set<Item*> items_;
};

#endif // INVENTORY_H
