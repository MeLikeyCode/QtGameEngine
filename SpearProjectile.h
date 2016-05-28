#ifndef SPEARPROJECTILE_H
#define SPEARPROJECTILE_H

#include "Projectile.h"

class QPointF;
class Map;

/// Represents a Projectile that is a spear which simply goes from the start position
/// towards the target position until it reaches range.
/// @author Abdullah Aghazadah
/// @date 5/24/16
/// @see Projectile
class SpearProjectile : public Projectile
{
public:
    SpearProjectile(QPointF start,
                    QPointF target,
                    double range,
                    double damage,
                    std::unordered_set<Entity*> noDamageList,
                    Map* map);
};

#endif // SPEARPROJECTILE_H
