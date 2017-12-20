#ifndef ITEMGOLD_H
#define ITEMGOLD_H

#include "NoTargetItem.h"

/// An Item of value.
/// Does not do anything, some shops and such will take some of this item
/// in exchange for goods/services.
class ItemGold : public NoTargetItem
{
public:
    ItemGold();

    virtual void use_();
};

#endif // ITEMGOLD_H
