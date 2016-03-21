#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"

class QPointF;

/// An abstract class that represents a melee weapon.
/// @author Abdullah Aghazadah
/// @date 10-18-15
class MeleeWeapon: public Weapon{
public:
    void setTip(QPointF point);
    void resetTip();
    QPointF tip();

private:
    QPointF tip_;

};

#endif // MELEEWEAPON_H
