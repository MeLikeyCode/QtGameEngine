#pragma once

#include "Projectile.h"

class QPointF;

namespace qge{

class Map;

/// Represents a Projectile that simply goes from the start position
/// towards the target position until it exceeds its range. When the SpearProjectile
/// exceeds its range, it will simply be destroyed.
/// @author Abdullah Aghazadah
/// @date 5/24/16
class SpearProjectile : public Projectile
{
public:
    SpearProjectile(double range, double damage);

    virtual void shootTowards(const QPointF& pos);

private:
    double range_;
    double distTravelledSoFar_;
};

}
