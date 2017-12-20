#include "NoTargetItem.h"

/// Uses the item.
void NoTargetItem::use()
{
    // delegate to concrete use_
    use_();

    // reduce number of charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}
