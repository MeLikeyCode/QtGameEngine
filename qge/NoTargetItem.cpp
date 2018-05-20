#include "NoTargetItem.h"

#include "NoTargetAbility.h"

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

/// Sets the ability that will be used when the item is used.
/// Can be nullptr. If the ability is nullptr, then the virtual use_() function will
/// be called when use() is called.
void NoTargetItem::setAbility(NoTargetAbility *ability)
{
    ability_ = ability;
    if (ability)
        setIcon(ability->icon());
}
