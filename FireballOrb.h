#ifndef FIREBALLORB_H
#define FIREBALLORB_H

#include "RangedWeapon.h"

class QPointF;
class Sound;

class FireballOrb : public RangedWeapon
{
public:
    FireballOrb();
    void attack(QPointF position);
private:
    Sound* soundEffect_;
};

#endif // FIREBALLORB_H
