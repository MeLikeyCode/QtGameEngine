#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "EquipableItem.h"
#include <QPointF>

/// Abstract class that represents a weapon that fires projectiles.
class RangedWeapon : public EquipableItem
{
public:
    RangedWeapon();

    virtual void attack(QPointF targetPoint) = 0;

    QPointF projectileSpawnPoint();
    void setProjectileSpawnPoint(QPointF point);
    void resetProjectileSpawnPoint();

private:
    QPointF projectileSpawnPoint_;
};

#endif // RANGEDWEAPON_H
