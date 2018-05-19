#include "CBDamage.h"

#include "Entity.h"
#include "STLWrappers.h"

using namespace qge;

CBDamage::CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo):
    amountToDamageEntityOne_(amountToDamageEntityOne),
    amountToDamageEntityTwo_(amountToDamageEntityTwo)
{

}

void CBDamage::onCollided(Entity *entityOne, Entity *entityTwo, const std::set<std::string>& doNotDamageTags, const std::set<Entity*>& doNotDamageEntities)
{
    // # if this collision is to be ignored, do nothing
    // ignore due to tag
    for (const std::string& tag : doNotDamageTags)
        if (entityOne->containsTag(tag) || entityTwo->containsTag(tag))
            return;
    // ignore due to specific entity
    if (STLWrappers::containsAny(doNotDamageEntities,{entityOne,entityTwo}))
        return;

    entityOne->setHealth(entityOne->health() - amountToDamageEntityOne_);
    entityTwo->setHealth(entityTwo->health() - amountToDamageEntityTwo_);
}
