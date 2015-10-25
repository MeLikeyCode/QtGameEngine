#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

// inherits
#include "Entity.h"

// pointer members
class Map;

// function arguments
class Sprite;
class QPointF;

/// An abstract class that defines a common interface for all weapon entities.
/// @author Abdullah Aghazadah
/// @date 10-18-15
class MeleeWeapon: public Entity{
public:
    // constructor
    MeleeWeapon();

    Entity* owner() const;
    void setOwner(Entity* entity);

    virtual void attackThrust() = 0; // makes this class abstract
//    virtual void attackThrust(int atAngle);
//    virtual void attackThrust(QPointF atPoint);

//    virtual void attackSwing();
//    virtual void attackSwing(int atAngle);
//    virtual void attackSwing(QPointF atPoint);

private:
    Entity* owner_;
};

#endif // MELEEWEAPON_H
