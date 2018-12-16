#pragma once

#include "Vendor.h"

namespace qge{

class Item;
class Entity;
class EquipableItem;
class NoTargetItem;
class EntityTargetItem;
class PointTargetItem;

/// Represents a collection of Items.
/// @author Abdullah Aghazadah
class Inventory: public QObject
{
    Q_OBJECT

    // Entity and Inventory reference each other. The association set up
    // code is in the Dynamic Entity, which needs private access to set the
    // pointer in Inventory.
    friend class Entity;
public:
    Inventory();
    ~Inventory();

    Entity* entity();

    void addItem(Item* item);
    void removeItem(Item* item);
    bool contains(Item* item);

    std::unordered_set<EquipableItem*> getEquipableItems();
    std::unordered_set<NoTargetItem*> getNoTargetItems();
    std::unordered_set<EntityTargetItem*> getEntityTargetItems();
    std::unordered_set<PointTargetItem*> getPointTargetItems();
    std::unordered_set<Item*> getItems();

signals:
    void itemAdded(Item* item);
    void itemRemoved(Item* item);

public slots:
    void onItemDying_(Entity* dyingItem);

private:
    Entity* entity_;
    std::unordered_set<Item*> items_;
};

}
