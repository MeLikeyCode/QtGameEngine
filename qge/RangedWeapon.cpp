#include "RangedWeapon.h"

#include "Sprite.h"
#include "EntitySprite.h"

using namespace qge;

/// Returns the point at which projectiles will be spawn.
/// This point is in local coordinates (relative to the RangedWeapon itself).
QPointF RangedWeapon::projectileSpawnPoint()
{
    return this->projectileSpawnPoint_;
}

void RangedWeapon::setProjectileSpawnPoint(QPointF point)
{
    this->projectileSpawnPoint_ = point;
}

/// Resets the projectile spawn point to be at the very center of the RangedWeapon's sprite.
void RangedWeapon::resetProjectileSpawnPoint()
{
    double length = sprite()->currentlyDisplayedFrame().width();
    double width = sprite()->currentlyDisplayedFrame().height();

    QPointF center;
    center.setX(length/2);
    center.setY(width/2);
    setProjectileSpawnPoint(center);
}
