#ifndef DRBDESTROYPROJECTILE_H
#define DRBDESTROYPROJECTILE_H

#include "DestReachedBehavior.h"

/// A DestReachedBehavior that simply destroys the projectile.
class DRBDestroyProjectile : public DestReachedBehavior
{
public:
    virtual void onDestinationReached(Projectile& projectile);
};

#endif // DRBDESTROYPROJECTILE_H
