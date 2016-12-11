#include "NoTargetAbility.h"
#include <cassert>
#include "Entity.h"

NoTargetAbility::NoTargetAbility(Entity &owner, Sprite *icon):
    Ability(owner,icon)
{
    // empty
}
