#ifndef CBDAMAGE_H
#define CBDAMAGE_H

#include "CollisionBehavior.h"
#include <unordered_set>

/// A CollisionBehavior that will damage one, both, or none of the colliding entities.
/// You can add "exceptions" (Entities that will not be damaged).
class CBDamage : public CollisionBehavior
{
public:
    CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo);

    void onCollided(Entity *entityOne, Entity *entityTwo);

    void addException(Entity* entity);

private:
    double amountToDamageEntityOne_;
    double amountToDamageEntityTwo_;
    std::unordered_set<Entity*> exceptions_;
};

#endif // CBDAMAGE_H
