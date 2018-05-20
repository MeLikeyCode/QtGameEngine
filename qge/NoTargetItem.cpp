#include "NoTargetItem.h"

#include "NoTargetAbility.h"
#include "TopDownSprite.h"

using namespace qge;

NoTargetItem::NoTargetItem(NoTargetAbility *ability):
    ability_(ability)
{
    setAbility(ability);
}

/// Uses the item.
void NoTargetItem::use()
{
    if (ability_){
        ability_->setOwner(owner());
        ability_->use();
    }
    else{
        use_();
    }

    // reduce number of charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}

/// Sets the ability of the item.
/// Giving the item an ability means setting the icon/sprite of the item to the
/// icon of the ability and using the ability when the item is used.
void NoTargetItem::setAbility(NoTargetAbility *ability)
{
    ability_ = ability;
    if (ability){
        setIcon(ability->icon());
        setSprite(new TopDownSprite(ability->icon()));
    }
}
