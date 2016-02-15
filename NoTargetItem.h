#ifndef NOTARGETITEM_H
#define NOTARGETITEM_H

#include "Item.h"

/// Represents an Item that does not require any targets to use. It can simply
/// be used.
/// @author Abdullah Aghazadah
/// @date 2/13/16
class NoTargetItem : public Item
{
public:
    NoTargetItem();
    void use();
};

#endif // NOTARGETITEM_H
