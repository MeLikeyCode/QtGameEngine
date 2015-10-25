#include "MeleeWeapon.h"
#include <cassert>
#include "Sprite.h"
#include "Map.h"

MeleeWeapon::MeleeWeapon():
    owner_(nullptr)
{

}

/// Returns (a pointer to) the Entity which owns this weapon.
Entity *MeleeWeapon::owner() const
{
    // make sure the weapon has an owner
    assert(owner_);

    return owner_;
}

/// Sets the owner of the weapon.
void MeleeWeapon::setOwner(Entity *entity)
{
    owner_ = entity;
}
