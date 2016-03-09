#ifndef PROJECTILECOLLISIONBEHAVIORDAMAGE_H
#define PROJECTILECOLLISIONBEHAVIORDAMAGE_H

#include "ProjectileCollisionBehavior.h"

class ProjectileCollisionBehaviorDamage : public ProjectileCollisionBehavior
{
public:
    virtual void onCollisionWith(std::unordered_set<Entity*> entities);
};

#endif // PROJECTILECOLLISIONBEHAVIORDAMAGE_H
