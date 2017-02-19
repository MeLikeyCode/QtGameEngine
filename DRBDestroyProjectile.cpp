#include "DRBDestroyProjectile.h"
#include "Projectile.h"

void DRBDestroyProjectile::onDestinationReached(Projectile &projectile)
{
    projectile.deleteLater();
}
