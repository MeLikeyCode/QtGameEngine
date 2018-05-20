#include "PointTargetItem.h"

#include "PointTargetAbility.h"
#include "TopDownSprite.h"

using namespace qge;

/// Constructs a PointTargetItem, optionally giving it the specified ability.
PointTargetItem::PointTargetItem(PointTargetAbility *ability):
    ability_(ability)
{
    setAbility(ability);
}

/// Uses the item on the specified point.
void PointTargetItem::use(const QPointF &point)
{
    if (ability_){
        ability_->setOwner(owner());
        ability_->use(point);
    }
    else{
        use_(point);
    }

    // reduce num charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}

/// Sets the ability of the item.
/// Giving the item an ability means setting the icon/sprite of the item to the
/// icon of the ability and using the ability when the item is used.
void PointTargetItem::setAbility(PointTargetAbility *ability)
{
    ability_ = ability;
    if (ability){
        setIcon(ability->icon());
        setSprite(new TopDownSprite(ability->icon()));
    }
}
