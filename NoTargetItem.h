#pragma once

#include "Item.h"

namespace qge{

/// Abstract class that represents an Item that does not require any targets to
/// use. It can simply be used.
/// @author Abdullah Aghazadah
/// @date 2/13/16
class NoTargetItem : public Item
{
public:
    void use();
protected:
    virtual void use_() = 0;
};

}
