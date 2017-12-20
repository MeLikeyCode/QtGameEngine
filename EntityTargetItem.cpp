#include "EntityTargetItem.h"

/// Uses the item on the specified entity.
void EntityTargetItem::use(Entity *onEntity)
{
    // delegate to concrete use_
    use_(onEntity);

    // reduce number of charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}
