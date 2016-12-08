#ifndef SHOPBEHAVIORNOCOST_H
#define SHOPBEHAVIORNOCOST_H

#include "ShopGuiBehavior.h"

/// Allows free buying at Shop.
class ShopBehaviorNoCost : public ShopGuiBehavior
{
public:
    virtual bool buy(Item* item, Inventory* buyer, Inventory* seller);
};

#endif // SHOPBEHAVIORNOCOST_H
