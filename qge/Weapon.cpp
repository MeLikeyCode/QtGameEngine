#include "Weapon.h"

using namespace qge;

Weapon::Weapon():
    castRange_(100)
{

}


/// Returns the cast range of the Weapon.
/// @see setCastRange().
double Weapon::castRange()
{
    return castRange_;
}

/// Sets the cast range of the Weapon. The cast range should be set to roughly the
/// distance that the weapon is effective from. Several things use this. For example,
/// when some entities are chasing an enemy, they will check if they are
/// close enough to use the weapon on the enemy, they do this by comparing their
/// distance from the enemy with the cast range of their weapon.
void Weapon::setCastRange(double range)
{
    castRange_ = range;
}
