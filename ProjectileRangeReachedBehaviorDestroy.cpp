#include "ProjectileRangeReachedBehaviorDestroy.h"
#include "Projectile.h"
#include "Map.h"

void ProjectileRangeReachedBehaviorDestroy::onRangeReached()
{
    // delete projectile
    projectile_->map()->removeEntity(projectile_);
    delete projectile_;
}
