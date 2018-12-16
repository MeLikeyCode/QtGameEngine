#pragma once

#include "Vendor.h"

#include "NoTargetItem.h"

namespace qge{

class ItemHealthPotion : public NoTargetItem
{
public:
    ItemHealthPotion(int amountToHealBy_);

    virtual void use_();

private:
    int amountToHealBy_;
};

}
