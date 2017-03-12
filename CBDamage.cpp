#include "CBDamage.h"
#include "Entity.h"

CBDamage::CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo):
    amountToDamageEntityOne_(amountToDamageEntityOne),
    amountToDamageEntityTwo_(amountToDamageEntityTwo)
{

}

void CBDamage::onCollided(Entity *entityOne, Entity *entityTwo)
{
    // if this collision is to be ignored, do nothing
    if (ignoredCollisions_.find(std::make_pair(entityOne,entityTwo)) != ignoredCollisions_.end())
        return;

    entityOne->setHealth(entityOne->health() - amountToDamageEntityOne_);
    entityTwo->setHealth(entityTwo->health() - amountToDamageEntityTwo_);
}

/// Specifies that the collision between these two entities should be ignored.
void CBDamage::addCollisionToIgnore(Entity *one, Entity *two)
{
    ignoredCollisions_.insert(std::make_pair(one,two));
    ignoredCollisions_.insert(std::make_pair(two,one));
}
