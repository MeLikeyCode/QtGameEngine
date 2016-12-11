#ifndef ENTITYTARGETABILITY_H
#define ENTITYTARGETABILITY_H

#include "Ability.h"

class Entity;

/// An Ability that requires a target Entity to be used on.
/// @author Abdullah Aghazadah
class EntityTargetAbility : public Ability {
public:
    EntityTargetAbility(Entity& owner, Sprite* icon);

    void use(Entity &onEntity);

protected:
    // template method pattern (use does some common work then delegates
    // to virtual function).
    virtual void useImplementation(Entity& onEntity) = 0;
};

#endif // ENTITYTARGETABILITY_H
