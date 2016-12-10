#include "MeleeWeapon.h"
#include <cassert>
#include "Sprite.h"
#include "Map.h"

#include "Inventory.h"

/// Set the tip of the MeleeWeapon.
/// @see MeleeWeapon::tip()
void MeleeWeapon::setTip(QPointF point)
{
    tip_ = point;
}

/// Sets the tip of the MeleeWeapon to be at x = widthOfWeapon and
/// y = heightOfWeapon/2.
void MeleeWeapon::resetTip()
{
    QPointF pt;
    pt.setX(sprite()->currentFrame().width());
    pt.setY(sprite()->currentFrame().height()/2);
    setTip(pt);
}

/// Returs the tip of the MeleeWeapon. The tip is the point that will be check
/// for collision with things.
QPointF MeleeWeapon::tip()
{
    return tip_;
}


/// Sets the CollisionBehavior of the MeleeWeapon.
void MeleeWeapon::setCollisionBehavior(CollisionBehavior *collisionBehavior)
{
    collisionBehavior_ = collisionBehavior;
}

/// Returns the CollisionBehavior of the MeleeWeapon.
CollisionBehavior *MeleeWeapon::collisionBehavior()
{
    return collisionBehavior_;
}
