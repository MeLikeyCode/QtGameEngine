#ifndef WEAPON_H
#define WEAPON_H

#include "EquipableItem.h"

class Weapon : public EquipableItem
{
public:
    virtual void attack(QPointF position) = 0;

    double castRange();
    void setCastRange(double range);

private:
    double castRange_; // how far can this weapon be used from?
};

#endif // WEAPON_H
