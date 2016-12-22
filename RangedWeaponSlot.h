#ifndef RANGEDWEAPONSLOT_H
#define RANGEDWEAPONSLOT_H

#include "Slot.h"

class EquipableItem;

class RangedWeaponSlot : public Slot
{
public:
    bool canBeEquipped(EquipableItem *item);
    void use();
};

#endif // RANGEDWEAPONSLOT_H
