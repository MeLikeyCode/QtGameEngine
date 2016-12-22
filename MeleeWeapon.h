#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"
#include <memory>
#include "CollisionBehavior.h"

class QPointF;

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

#endif // MELEEWEAPON_H
