#include "DRBDestroyProjectile.h"

#include "Projectile.h"

using namespace qge;

void DRBDestroyProjectile::onDestinationReached(Projectile &projectile)
{
    projectile.deleteLater();
}
