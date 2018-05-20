#include "EntityTargetAbility.h"

using namespace qge;

EntityTargetAbility::EntityTargetAbility(Entity *owner, QPixmap icon):
    Ability(owner,icon)
{
    // empty
}

/// Uses the EntityTargetAbility on the specified Entity.
void EntityTargetAbility::use(Entity &onEntity)
{
    // ensure *owner* has not died yet
    assertOwnerIsAlive_();

    // delgate to actual class
    useImplementation(onEntity);
}
