#pragma once

#include "NoTargetItem.h"

namespace qge{

/// An Item of value.
/// Does not do anything, some shops and such will take some of this item
/// in exchange for goods/services.
class ItemGold : public NoTargetItem
{
public:
    ItemGold();

    virtual void use_();
};

}
