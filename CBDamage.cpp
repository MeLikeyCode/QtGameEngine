#include "CBDamage.h"
#include "Entity.h"

CBDamage::CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo):
    amountToDamageEntityOne_(amountToDamageEntityOne),
    amountToDamageEntityTwo_(amountToDamageEntityTwo)
{

}

void CBDamage::onCollided(Entity *entityOne, Entity *entityTwo)
{
    // damage entity one if not in exceptions
    if (exceptions_.find(entityOne) == exceptions_.end())
        entityTwo->damage(entityOne,amountToDamageEntityOne_);
    // damage entity two if not in exceptions
    if (exceptions_.find(entityTwo) == exceptions_.end())
        entityOne->damage(entityTwo,amountToDamageEntityTwo_);
}

/// Adds the specified Entity to the "exceptions" list of the CBDamage.
/// Entities in the exceptions list will not be damaged.
void CBDamage::addException(Entity *entity)
{
    exceptions_.insert(entity);
}
