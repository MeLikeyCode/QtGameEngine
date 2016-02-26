#include "RangedWeaponSlot.h"
#include "RangedWeapon.h"
#include <QLineF>
#include "Inventory.h"

RangedWeaponSlot::RangedWeaponSlot()
{
}

bool RangedWeaponSlot::canBeEquipped(EquipableItem *item)
{
    // only RangeWeapons can be equipped
    RangedWeapon* asRW = dynamic_cast<RangedWeapon*>(item);
    return asRW;
}

void RangedWeaponSlot::use()
{
    // call attack of the RangeWeapon passing in a point ahead of the entity

    Entity* owner = this->item_->inventory()->entity();
    int RANGE = 400; //TODO: get from item_

    QLineF line(owner->pointPos(),QPointF(-5,-5));
    line.setAngle(-1 * owner->facingAngle());
    line.setLength(RANGE);

    QPointF target = line.p2();

    RangedWeapon* asRW = dynamic_cast<RangedWeapon*>(item_);
    asRW->attack(target);

}
