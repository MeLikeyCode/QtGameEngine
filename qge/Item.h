#pragma once

#include "Vendor.h"

#include "Entity.h"

namespace qge{

class Inventory;

/// Abstract class that represents any Item (such as HealthPotion, Axe, etc..)
/// that can be in an Inventory or lying around on the Map. If an item is in an
/// Inventory, the Inventory() function will return that Inventory, otherwise
/// if the Item is on the ground, it will return nullptr.
/// @author Abdullah Aghazadah
/// @date 2/13/16
class Item: public Entity
{
    Q_OBJECT
    // An Item can be inside an Inventory.
    // When an Item is removed from the Inventory, the Inventory needs to
    // set the private inventory_ pointer to nullptr
    friend class Inventory;
public:
    Inventory* inventory();
    Entity* owner();
    void setInventory(Inventory* inv);
    int numOfCharges();
    void setNumOfCharges(int n);
    void setDescription(std::string description);
    std::string description();
    QPixmap icon();
    void setIcon(const QPixmap& pixmap);

signals:
    /// Emitted when the number of charges of the items changes (either increases or decreases).
    void numChargesChanged(Item* sender);

protected:
    Item(); // constructor protected so that objects cannot be instantiated, but subclasses
            // can still use the constructor to set up the base class portion of the state

private:
    Inventory* inventory_; // the Inventory, nullptr means on ground
    std::string description_;
    int numOfCharges_;
    QPixmap icon_; // optional, guis will use sprite if no icon specified
};

}
