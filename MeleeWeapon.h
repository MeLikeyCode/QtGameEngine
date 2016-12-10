#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"

class QPointF;
class CollisionBehavior;

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
    CollisionBehavior* collisionBehavior_;

};

#endif // MELEEWEAPON_H
