#pragma once

#include "Item.h"

namespace qge{

class NoTargetAbility;

/// Abstract class that represents an Item that does not require any targets to
/// use. It can simply be used.
///
/// Implementation details
/// ----------------------
/// If the item has an ability assigned (via setAbility() or passed into the ctor)
/// then that ability will be used upon use(), other wise, the virtual use_() function will be
/// called. When the ability of an item is set, its icon/sprite will be changed to the icon of the ability.
/// In other words, by simply giving an item an Ability, you are also giving it an icon and a sprite!
/// @author Abdullah Aghazadah
/// @date 2/13/16
class NoTargetItem : public Item
{
public:
    NoTargetItem(NoTargetAbility* ability=nullptr);

    void use();
    void setAbility(NoTargetAbility* ability);

protected:
    virtual void use_() = 0;

private:
    NoTargetAbility* ability_;
};

}
