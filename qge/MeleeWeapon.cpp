#include "MeleeWeapon.h"

#include "Sprite.h"
#include "Map.h"
#include "EntitySprite.h"
#include "Inventory.h"

using namespace qge;

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
    pt.setX(sprite()->currentlyDisplayedFrame().width());
    pt.setY(sprite()->currentlyDisplayedFrame().height()/2);
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
    // release old collision behavior
    collisionBehavior_.release();

    // grab new one
    collisionBehavior_.reset(collisionBehavior);
}

/// Returns the CollisionBehavior of the MeleeWeapon.
CollisionBehavior *MeleeWeapon::collisionBehavior()
{
    return collisionBehavior_.get();
}
