#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"

class Inventory;

/// Abstract class that represents any Item (such as HealthPotion, Axe, etc..)
/// that can be in an Inventory or lying around on the Map. An Item can either be
/// in an Inventory or on the ground. If an item is in an Inventory, the Inventory()
/// function will return that Inventory, otherwies if the Item is on the ground,
/// it will return nullptr.
/// @author Abdullah Aghazadah
/// @date 2/13/16
class Item: public Entity
{
public:
    Item();

    Inventory* inventory();
    void setInventory(Inventory* inv);
    int numOfCharges();
    void setNumOfCharges(int n);

private:
    Inventory* inventory_; // the Inventory, nullptr means on ground
    int numOfCharges_;
};

#endif // ITEM_H
