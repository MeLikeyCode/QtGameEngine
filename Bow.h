#ifndef BOW_H
#define BOW_H

#include "RangedWeapon.h"

class QPointF;
class Sound;

class Bow : public RangedWeapon
{
public:
    Bow();

    void attack(QPointF targetPoint);

private:
    Sound* soundEffect_;
};

#endif // BOW_H
