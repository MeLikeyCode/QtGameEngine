#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "EquipableItem.h"
#include <QPixmap>

class Map;
class Sprite;
class QPointF;

/// An abstract class that represents a MeleeWeapon EquipableItem.
/// @author Abdullah Aghazadah
/// @date 10-18-15
class MeleeWeapon: public EquipableItem{
public:
    // constructor
    MeleeWeapon();

    virtual void attack() = 0; // makes this class abstract

    void setTip(QPointF point);
    void resetTip();
    QPointF tip();

private:
    QPointF tip_;

};

#endif // MELEEWEAPON_H
