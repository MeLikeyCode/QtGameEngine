#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"

class QPointF;

/// Abstract class that represents a weapon that can fire projectiles.
class RangedWeapon : public Weapon
{
public:
    QPointF projectileSpawnPoint();
    void setProjectileSpawnPoint(QPointF point);
    void resetProjectileSpawnPoint();

private:
    QPointF projectileSpawnPoint_;
};

#endif // RANGEDWEAPON_H
