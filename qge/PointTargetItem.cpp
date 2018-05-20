#include "PointTargetItem.h"

#include "PointTargetAbility.h"

using namespace qge;

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

/// Sets the ability that will be used when the item is used.
/// Can be nullptr. If the ability is nullptr, then the virtual use_() function will
/// be called when use() is called.
void PointTargetItem::setAbility(PointTargetAbility *ability)
{
    ability_ = ability;
    if (ability)
        setIcon(ability->icon()->currentFrame());
}
