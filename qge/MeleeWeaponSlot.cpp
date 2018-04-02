#include "MeleeWeaponSlot.h"

#include "MeleeWeapon.h"

using namespace qge;

bool MeleeWeaponSlot::canBeEquipped(EquipableItem *item)
{
    // MeeleeWeaponSlot can only equip MeleeWeapons
    MeleeWeapon* asMeleeWeapon = dynamic_cast<MeleeWeapon*>(item);
    return asMeleeWeapon;
}

void MeleeWeaponSlot::use()
{
    // call attack on the MeleeWeapon targetting 200 in front of the owner
    MeleeWeapon* asMeleeWeapon = dynamic_cast<MeleeWeapon*>(item_);
    QLineF line(owner()->pos(),QPointF(-100,-100));
    line.setAngle(-1*owner()->facingAngle());
    line.setLength(200);
    asMeleeWeapon->attack(line.p2());
}
