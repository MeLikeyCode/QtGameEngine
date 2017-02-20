#ifndef SPEARPROJECTILE_H
#define SPEARPROJECTILE_H

#include "Projectile.h"

class QPointF;
class Map;

/// Represents a Projectile that is a spear which simply goes from the start position
/// towards the target position until it exceeds its range. When the SpearProjectile
/// exceeds its range, it will simply be destroyed.
/// @author Abdullah Aghazadah
/// @date 5/24/16
class SpearProjectile : public Projectile
{
public:
    SpearProjectile(double range, double damage, std::unordered_set<Entity *> noDmgList);
public slots:
    void onMoved_(Entity* entity, QPointF fromPos, QPointF toPos);
private:
    double range_;
    double distTravelledSoFar_;
};

#endif // SPEARPROJECTILE_H
