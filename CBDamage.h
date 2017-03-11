#ifndef CBDAMAGE_H
#define CBDAMAGE_H

#include "CollisionBehavior.h"
#include <unordered_set>
#include <utility>
#include "Entity.h"

/// A CollisionBehavior that will damage one, both, or none of the colliding entities.
/// You can specifiy collisions to ignore.
/// @author Abdullah Aghazadah
class CBDamage : public CollisionBehavior
{
public:
    CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo);

    void onCollided(Entity *entityOne, Entity *entityTwo);

    void addCollisionToIgnore(Entity* one, Entity* two);

private:
    double amountToDamageEntityOne_;
    double amountToDamageEntityTwo_;
    std::unordered_set<std::pair<Entity*,Entity*>> ignoredCollisions_;
};

#endif // CBDAMAGE_H
