#include "NoTargetAbility.h"
#include <cassert>
#include "DynamicEntity.h"

NoTargetAbility::NoTargetAbility(DynamicEntity *owner, Sprite *icon):
    Ability(owner,icon)
{
    // empty
}
