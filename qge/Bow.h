#pragma once

#include "Vendor.h"

#include "RangedWeapon.h"

class QPointF;

namespace qge{

class Sound;

class Bow : public RangedWeapon
{
public:
    Bow();

    void attack(QPointF targetPoint);

private:
    Sound* soundEffect_;
};

}
