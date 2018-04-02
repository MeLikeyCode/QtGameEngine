#pragma once

#include "Weapon.h"

class QPointF;

namespace qge{

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

}
