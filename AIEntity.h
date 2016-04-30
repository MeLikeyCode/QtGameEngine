#ifndef ENEMY_H
#define ENEMY_H

#include "DynamicEntity.h"
#include <set>

class Weapon;

/// Represents an Entity that can move and think on its own. AIEntities will
/// attack any enemies in their field of view.
/// @author Abdullah Aghazadah
/// @date 4/18/16
class AIEntity: public DynamicEntity
{
    Q_OBJECT
public:
    AIEntity();
    ~AIEntity();

    Weapon* defaultWeapon();
    void setDefaultWeapon(Weapon* weapon);

    void addEnemy(int groupID);
    bool isEnemy(int groupID);
    bool isEnemy(Entity* entity);
    void removeEnemy(int groupID);

public slots:
    void checkFov_();
    void startCheckingFov_();
    void swingIfInRange_();
    void onTargetDelete_();
private:
    QTimer* timerCheckFov_;
    QTimer* attackTimer_;

    Weapon* defaultWeapon_;
    Entity* targetEntity_;

    std::set<int> enemyGroups_;
};

#endif // ENEMY_H
