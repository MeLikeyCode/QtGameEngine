#include "WeaponSlot.h"

#include "Weapon.h"

using namespace qge;

bool WeaponSlot::canBeEquipped(EquipableItem *item)
{
    // WeaponSlot can only equip Weapons
    Weapon* asWeapon = dynamic_cast<Weapon*>(item);
    return asWeapon;
}

void WeaponSlot::use()
{
    // call attack on the Weapon
    Weapon* asWeapon = dynamic_cast<Weapon*>(item_);
    QLineF line(owner()->pos(),QPointF(-100,-100));
    line.setAngle(-1*owner()->facingAngle());
    line.setLength(200);
    asWeapon->attack(line.p2());
}
