#ifndef CBDAMAGE_H
#define CBDAMAGE_H

#include "CollisionBehavior.h"

/// A CollisionBehavior that will damage one, both, or none of the colliding entities.
class CBDamage : public CollisionBehavior
{
public:
    CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo);

    void onCollided(Entity *entityOne, Entity *entityTwo);

private:
    double amountToDamageEntityOne_;
    double amountToDamageEntityTwo_;
};

#endif // CBDAMAGE_H
