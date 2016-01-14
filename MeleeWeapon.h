#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

// inherits
#include "Entity.h"

// pointer members
class Map;

// function arguments
class Sprite;
class QPointF;

/// An abstract class that represents a weapon entity.
/// @author Abdullah Aghazadah
/// @date 10-18-15
class MeleeWeapon: public Entity{
public:
    // constructor
    MeleeWeapon();

    Entity* owner() const;
    void setOwner(Entity* entity);

    virtual void attack() = 0; // makes this class abstract
    virtual QPointF attachmentPoint() = 0;

private:
    Entity* owner_;
};

#endif // MELEEWEAPON_H
