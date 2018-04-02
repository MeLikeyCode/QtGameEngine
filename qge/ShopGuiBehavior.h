#pragma once

namespace qge{

class Inventory;
class Item;

class ShopGuiBehavior
{
public:
    /// Executed when something is attempted to be bought by the ShopGui.
    /// Do whats necessary (i.e. attempt to take gold or something from the buyer)
    /// and then return true to indicate that the buying was succesfull (i.e. you
    /// succesfully took transfered gold from buyer to seller) or not (i.e.
    /// the buyer did not have enough gold).
    virtual bool buy(Item* item, Inventory* seller, Inventory* buyer) = 0;
};

}
