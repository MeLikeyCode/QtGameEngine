#pragma once

#include "Vendor.h"

#include "RangedWeapon.h"

class QPointF;

namespace qge{

class Sound;

/// A RangedWeapon that on attack() will launch a fireball towards the specified position.
/// @author Abdullah Aghazadah
class FireballLauncher : public RangedWeapon
{
public:
    FireballLauncher();
    void attack(QPointF position);
private:
    Sound* soundEffect_;
};

}
