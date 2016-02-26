#ifndef BOW_H
#define BOW_H

#include "RangedWeapon.h"

class QPointF;

class Bow : public RangedWeapon
{
public:
    Bow();

    void attack(QPointF targetPoint);
};

#endif // BOW_H
