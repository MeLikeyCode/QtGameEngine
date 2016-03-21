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

/// Sets the tip of the MeleeWeapon to be at x = length() and y = width()/2
/// (i.e. at the very lengthwise, and middle widthwise of the sprite).
void MeleeWeapon::resetTip()
{
    QPointF pt;
    pt.setX(length());
    pt.setY(width()/2);
    setTip(pt);
}

/// Returs the tip of the MeleeWeapon. The tip is the point that will be check
/// for collision with things.
QPointF MeleeWeapon::tip()
{
    return tip_;
}
