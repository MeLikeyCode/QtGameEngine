#include "PointTargetAbility.h"

using namespace qge;

PointTargetAbility::PointTargetAbility(Entity *owner, QPixmap icon):
    Ability(owner,icon)
{
    // empty
}

/// Uses the PointTargetAbility at the specified point.
void PointTargetAbility::use(const QPointF &atPoint)
{
    // make sure owner is still alive
    assertOwnerIsAlive_();

    // delegate to implementation
    useImplementation(atPoint);
}
