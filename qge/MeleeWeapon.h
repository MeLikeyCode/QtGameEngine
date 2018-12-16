#pragma once

#include "Vendor.h"

#include "Weapon.h"
#include "CollisionBehavior.h"

class QPointF;

namespace qge{

/// An abstract class that represents a melee weapon.
/// @author Abdullah Aghazadah
/// @date 10-18-15
class MeleeWeapon: public Weapon{
public:
    void setTip(QPointF point);
    void resetTip();
    QPointF tip();

    void setCollisionBehavior(CollisionBehavior* collisionBehavior);
    CollisionBehavior* collisionBehavior();

private:
    QPointF tip_;
    std::unique_ptr<CollisionBehavior> collisionBehavior_;
};

}
