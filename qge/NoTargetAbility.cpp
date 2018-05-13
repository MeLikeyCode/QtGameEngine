#include "NoTargetAbility.h"
#include <cassert>
#include "Entity.h"

using namespace qge;

NoTargetAbility::NoTargetAbility(Entity *owner, Sprite *icon):
    Ability(owner,icon)
{
    // empty
}

/// Uses the NoTargetAbility.
void NoTargetAbility::use()
{
    // make sure *owner* has not died yet
    assertOwnerIsAlive_();

    // delgate to implementation
    useImplementation();
}
