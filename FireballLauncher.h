#ifndef FIREBALLLAUNCHER_H
#define FIREBALLLAUNCHER_H

#include "RangedWeapon.h"

class QPointF;
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

#endif // FIREBALLLAUNCHER_H
