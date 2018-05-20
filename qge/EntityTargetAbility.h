#pragma once

#include "Ability.h"

namespace qge{

class Entity;

/// An Ability that requires a target Entity to be used on.
/// @author Abdullah Aghazadah
class EntityTargetAbility : public Ability {
public:
    EntityTargetAbility(Entity* owner=nullptr, QPixmap icon=QPixmap());

    void use(Entity &onEntity);

protected:
    // template method pattern (use does some common work then delegates
    // to virtual function).
    virtual void useImplementation(Entity& onEntity) = 0;
};

}
