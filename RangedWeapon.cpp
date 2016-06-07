#include "RangedWeapon.h"
#include "Sprite.h"

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

/// Resets the projectile spawn point to be at the very center of the RangedWeapon.
void RangedWeapon::resetProjectileSpawnPoint()
{
    double length = sprite()->currentFrame().width();
    double width = sprite()->currentFrame().height();

    QPointF center;
    center.setX(length/2);
    center.setY(width/2);
    setProjectileSpawnPoint(center);
}
