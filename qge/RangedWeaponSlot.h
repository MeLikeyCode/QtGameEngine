#pragma once

#include "Vendor.h"

#include "Slot.h"

namespace qge{

class EquipableItem;

class RangedWeaponSlot : public Slot
{
public:
    bool canBeEquipped(EquipableItem *item);
    void use();
};

}
