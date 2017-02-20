#ifndef DRBDESTROYPROJECTILE_H
#define DRBDESTROYPROJECTILE_H

#include "DestReachedBehavior.h"

/// A DestReachedBehavior that simply destroys the projectile.
/// @author Abdullah Aghazadah
/// @date 2/18/17
class DRBDestroyProjectile : public DestReachedBehavior
{
public:
    virtual void onDestinationReached(Projectile& projectile);
};

#endif // DRBDESTROYPROJECTILE_H
