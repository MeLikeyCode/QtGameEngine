#include "WeaponSlot.h"

#include "Weapon.h"

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
    asWeapon->attack(QPointF(0,0));
}
