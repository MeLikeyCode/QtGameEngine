#include "Item.h"
#include <cassert>
#include "Inventory.h"
#include "Sprite.h"

Item::Item():
    inventory_(nullptr),
    numOfCharges_(1),
    description_("Item has no description")
{
    // empty constructor body
}

/// Returns the Inventory that this Item belongs to. If this Item is on the
/// ground (i.e. does not belong to any Inventory), will return nullptr.
Inventory *Item::inventory()
{
    return inventory_;
}

/// Set the Inventory that this Item should belong to.
/// Simply delegates to Inventory::addItem(this)
/// @see Inventory::addItem(Item*)
/// If you want to drop an Item on the ground, do setInventory(nullptr).
void Item::setInventory(Inventory *inv)
{
    // set on ground
    if (inv == nullptr){
        setPos(mapToMap(pos()));  // put pos relative to map
        setParentEntity(nullptr);
        inventory_ = inv;
    }
    // put in inv (delgate to Inventory)
    else {
        inv->addItem(this);
    }
}

/// Returns the number of charges the Item has.
/// -1 means infinite.
int Item::numOfCharges()
{
    return numOfCharges_;
}

/// Sets the number of charges the Item has.
/// -1 means infinite.
void Item::setNumOfCharges(int n)
{
    numOfCharges_ = n;
    if (numOfCharges_ == 0)
        deleteLater();
}

/// Sets the description text of the Item.
/// The description text is shown in places like shops, etc...
void Item::setDescription(std::string description)
{
    description_ = description;
}

/// Returns the description text of the Item.
/// @see setDescription()
std::string Item::description()
{
    return description_;
}

/// Returns the owner of the item (which is just the owner of the inventory that the item is in).
/// Returns nullptr if the item isn't in an inventory or if the inventory doesn't belong to anyone.
Entity *Item::owner()
{
    Inventory* inv = inventory();
    if (inv == nullptr)
        return nullptr;

    Entity* ow = inv->entity();
    return ow;
}
