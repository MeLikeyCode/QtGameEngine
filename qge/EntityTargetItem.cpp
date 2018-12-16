#include "EntityTargetItem.h"

#include "EntityTargetAbility.h"
#include "TopDownSprite.h"

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

/// Sets the ability of the item.
/// Giving the item an ability means setting the icon/sprite of the item to the
/// icon of the ability and using the ability when the item is used.
void EntityTargetItem::setAbility(EntityTargetAbility *ability)
{
    ability_ = ability;
    if (ability){
        setIcon(ability->icon());
        setSprite(new TopDownSprite(ability->icon()));
    }
}
