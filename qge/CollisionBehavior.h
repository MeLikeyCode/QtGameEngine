#pragma once

#include "Vendor.h"

namespace qge{

class Entity;

/// An interface for behavior objects that decides what happens when two Entities
/// collide.
/// @author Abdullah Aghazadah
/// @date 12/8/16
class CollisionBehavior
{
public:
    /// This function will be called on the CollisionBehavior whenever two entities
    /// of interest have collided. Some concrete CollisionBehaviors will damage one or both entity,
    /// some will heal, some will push one of the entities back, etc...
    /// @param entityOne
    /// @param entityTwo the entities that have collided.
    /// @param doNotDamageTags Entities with these tags should not be damaged!
    /// @param doNotDamageEntities These specific entities (regarless of tags) should not be damaged!
    /// @attention Implementations of this function need to ensure that entities with any of the
    /// 'doNotDamageTags' or entities in 'doNotDamageEntities' are in fact not damaged!
    virtual void onCollided(Entity* entityOne,
                            Entity* entityTwo,
                            const std::set<std::string>& doNotDamageTags,
                            const std::set<Entity*>& doNotDamageEntities) = 0;
};

}
