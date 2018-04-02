#pragma once

#include "Slot.h"

namespace qge{

class EquipableItem;

/// Represents a Slot that can hold a MeleeWeapon.
class MeleeWeaponSlot : public Slot
{
public:
    bool canBeEquipped(EquipableItem* item);
    void use();
};

}
