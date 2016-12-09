#include "CBDamage.h"
#include "Entity.h"

CBDamage::CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo):
    damageAmount_(amountToDamageEntityOne),
    damageEntityOne_(amountToDamageEntityTwo),
    damageEntityTwo_(damageEntityTwo)
{

}

void CBDamage::onCollided(Entity *entityOne, Entity *entityTwo)
{
    entityTwo->damage(entityOne,amountToDamageEntityOne_);
    entityOne->damage(entityTwo,amountToDamageEntityTwo_);
}
