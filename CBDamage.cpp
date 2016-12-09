#include "CBDamage.h"
#include "Entity.h"

CBDamage::CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo):
    amountToDamageEntityOne_(amountToDamageEntityOne),
    amountToDamageEntityTwo_(amountToDamageEntityTwo)
{

}

void CBDamage::onCollided(Entity *entityOne, Entity *entityTwo)
{
    entityTwo->damage(entityOne,amountToDamageEntityOne_);
    entityOne->damage(entityTwo,amountToDamageEntityTwo_);
}
