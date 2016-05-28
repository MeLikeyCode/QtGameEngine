#ifndef WEAPONSLOT_H
#define WEAPONSLOT_H

#include "Slot.h"

class EquipableItem;

class WeaponSlot : public Slot
{
public:
    WeaponSlot();

    bool canBeEquipped(EquipableItem* item);
    void use();
};

#endif // WEAPONSLOT_H
