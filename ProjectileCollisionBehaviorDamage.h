#ifndef PROJECTILECOLLISIONBEHAVIORDAMAGE_H
#define PROJECTILECOLLISIONBEHAVIORDAMAGE_H

#include "ProjectileCollisionBehavior.h"
#include <QPointer>

class ProjectileCollisionBehaviorDamage : public ProjectileCollisionBehavior
{
public:
    ProjectileCollisionBehaviorDamage(int damageAmount);

    virtual void onCollisionWith(std::unordered_set<QPointer<Entity>> entities);
private:
    double damage_;
};

#endif // PROJECTILECOLLISIONBEHAVIORDAMAGE_H
