#pragma once

#include "EquipableItem.h"

namespace qge{

/// An abstract class that represents an EquipableItem that can
/// attack towards a position.
/// @author Abdullah Aghazadah
class Weapon : public EquipableItem
{
public:
    Weapon();
    virtual void attack(QPointF position) = 0;

    double castRange();
    void setCastRange(double range);

private:
    double castRange_; // how far can this weapon be used from?
};

}
