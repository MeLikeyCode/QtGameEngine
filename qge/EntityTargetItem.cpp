#include "EntityTargetItem.h"

#include <cassert>

#include "EntityTargetAbility.h"

using namespace qge;

EntityTargetItem::EntityTargetItem(EntityTargetAbility *ability):
    ability_(ability)
{
    setAbility(ability);
}

/// Uses the item on the specified entity.
void EntityTargetItem::use(Entity *onEntity)
{
    assert(onEntity);

    if (ability_){
        ability_->setOwner(owner());
        ability_->use(*onEntity);
    }
    else{
        use_(onEntity);
    }

    // reduce number of charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}

/// Sets the ability that will be used when the item is used.
/// Can be nullptr. If the ability is nullptr, then the virtual use_() function will
/// be called when use() is called.
void EntityTargetItem::setAbility(EntityTargetAbility *ability)
{
    ability_ = ability;
    if (ability)
        setIcon(ability->icon()->currentFrame());
}
