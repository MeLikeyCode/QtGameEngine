#include "Item.h"
#include <cassert>
#include "Inventory.h"

Item::Item()
{
    // initialize:
    inventory_ = nullptr;
    numOfCharges_ = 1;
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
void Item::setInventory(Inventory *inv)
{
    // set on ground
    if (inv == nullptr){
        sprite()->setVisible(true);         // make visible
        sprite()->setScale(0.75);           // make smaller
        setPointPos(mapToMap(pointPos()));  // put pos relative to map
        setParentEntity(nullptr);
        inventory_ = inv;
    }
    // put in inv
    else {
        // make sure not already in inventory
        assert(inventory_ == nullptr);

        sprite()->setVisible(false);        // make invisble
        inventory_ = inv;
    }
}

/// Returns the number of charges the Item has.
int Item::numOfCharges()
{
    return numOfCharges_;
}

/// Sets the number of charges the Item has.
void Item::setNumOfCharges(int n)
{
    numOfCharges_ = n;
}
