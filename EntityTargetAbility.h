#ifndef ENTITYTARGETABILITY_H
#define ENTITYTARGETABILITY_H

#include "Ability.h"

class Entity;

class EntityTargetAbility : public Ability {
public:
    EntityTargetAbility(DynamicEntity* owner, Sprite* icon);

    virtual void use(Entity* onEntity) = 0;
};

#endif // ENTITYTARGETABILITY_H
