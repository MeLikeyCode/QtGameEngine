#include "PointTargetItem.h"

using namespace qge;

/// Uses the item on the specified point.
void PointTargetItem::use(const QPointF &point)
{
    // delegate to concrete use_
    use_(point);

    // reduce num charges
    if (numOfCharges() != -1) // -1 charges represents an infinite number of charges
        setNumOfCharges(numOfCharges()-1);
}
