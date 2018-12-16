#pragma once

#include "Vendor.h"

#include "CollisionBehavior.h"
#include "Entity.h"

namespace qge{

/// A CollisionBehavior that will damage one, both, or none of the colliding entities.
/// @author Abdullah Aghazadah
class CBDamage : public CollisionBehavior
{
public:
    CBDamage(double amountToDamageEntityOne, double amountToDamageEntityTwo);

    void onCollided(Entity *entityOne,
                    Entity *entityTwo,
                    const std::set<std::string>& doNotDamageTags,
                    const std::set<Entity*>& doNotDamageEntities
                    ) override;

private:
    double amountToDamageEntityOne_;
    double amountToDamageEntityTwo_;
};

}
