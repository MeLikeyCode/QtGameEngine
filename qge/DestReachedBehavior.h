#pragma once

namespace qge{

class Projectile;

/// An object that determines what happens when a Projectile
/// has reached its destination.
///
/// Concrete Projectiles may destroy the projectile, explode
/// it, etc...
///
/// @author Abdullah Aghazadah
/// @date 2-18-17
class DestReachedBehavior
{
public:
    /// Executed when the specified Projectile has reached its destination.
    /// Concrete DestReachedBehaviors may destroy the projectile, explode it,
    /// etc...
    virtual void onDestinationReached(Projectile& projectile) = 0;
};

}
