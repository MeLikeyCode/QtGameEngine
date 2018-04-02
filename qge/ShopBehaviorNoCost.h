#pragma once

#include "ShopGuiBehavior.h"

namespace qge{

/// Allows free buying at Shop.
class ShopBehaviorNoCost : public ShopGuiBehavior
{
public:
    virtual bool buy(Item* item, Inventory* buyer, Inventory* seller);
};

}
