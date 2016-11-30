#include "Ability.h"

Ability::Ability(Entity* owner, Sprite* icon):
    owner_(owner),
    icon_(icon)
{
    // empty ctor body
}

Entity *Ability::owner()
{
    return owner_;
}

void Ability::setOwner(Entity *owner)
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
