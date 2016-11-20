#include "Ability.h"

Ability::Ability(DynamicEntity* owner, Sprite* icon):
    owner_(owner),
    icon_(icon)
{
    // empty ctor body
}

DynamicEntity *Ability::owner()
{
    return owner_;
}

void Ability::setOwner(DynamicEntity *owner)
{
    owner_ = owner;
}

Sprite *Ability::icon()
{
    return icon_;
}

void Ability::setIcon(Sprite *sprite)
{
    icon_ = sprite;
}
