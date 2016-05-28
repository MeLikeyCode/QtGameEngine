#ifndef MELEEWEAPONSLOT_H
#define MELEEWEAPONSLOT_H

#include "Slot.h"

class EquipableItem;

/// Represents a Slot that can hold a MeleeWeapon.
class MeleeWeaponSlot : public Slot
{
public:
    MeleeWeaponSlot();

    bool canBeEquipped(EquipableItem* item);
    void use();
};

#endif // MELEEWEAPONSLOT_H
