#ifndef ENEMY_H
#define ENEMY_H

#include "DynamicEntity.h"

class Weapon;

class Enemy: public DynamicEntity
{
    Q_OBJECT
public:
    Enemy();

    Weapon* defaultWeapon();
    void setDefaultWeapon(Weapon* weapon);

public slots:
    void checkFov_();
    void startCheckingFov_();
    void swingIfInRange_();
private:
    QTimer* timerCheckFov_;
    QTimer* timerStartCheckingFov_;
    QTimer* attackTimer_;

    Weapon* defaultWeapon_;
    Entity* targetEntity_;


};

#endif // ENEMY_H
