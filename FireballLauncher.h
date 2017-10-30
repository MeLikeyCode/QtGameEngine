#ifndef FIREBALLLAUNCHER_H
#define FIREBALLLAUNCHER_H

#include "RangedWeapon.h"

class QPointF;
class Sound;

class FireballLauncher : public RangedWeapon
{
public:
    FireballLauncher();
    void attack(QPointF position);
private:
    Sound* soundEffect_;
};

#endif // FIREBALLLAUNCHER_H
