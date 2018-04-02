#pragma once

#include "DestReachedBehavior.h"

namespace qge{

/// A DestReachedBehavior that simply destroys the projectile.
/// @author Abdullah Aghazadah
/// @date 2/18/17
class DRBDestroyProjectile : public DestReachedBehavior
{
public:
    virtual void onDestinationReached(Projectile& projectile);
};

}
