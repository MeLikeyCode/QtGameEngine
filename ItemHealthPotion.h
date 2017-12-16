#ifndef ITEMHEALTHPOTION_H
#define ITEMHEALTHPOTION_H

#include "NoTargetItem.h"

class ItemHealthPotion : public NoTargetItem
{
public:
    ItemHealthPotion(int amountToHealBy_);

    virtual void use();

private:
    int amountToHealBy_;
};

#endif // ITEMHEALTHPOTION_H
