#ifndef PROJECTILECOLLISIONBEHAVIORDAMAGE_H
#define PROJECTILECOLLISIONBEHAVIORDAMAGE_H

#include "ProjectileCollisionBehavior.h"

class ProjectileCollisionBehaviorDamage : public ProjectileCollisionBehavior
{
public:
    ProjectileCollisionBehaviorDamage();

    virtual void onCollisionWith(std::unordered_set<Entity*> entities);
private:
    double damage_;
};

#endif // PROJECTILECOLLISIONBEHAVIORDAMAGE_H
