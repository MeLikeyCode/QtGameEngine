#ifndef ENEMY_H
#define ENEMY_H

#include "DynamicEntity.h"
#include <set>

class Weapon;

/// Represents an Entity that can move and think on its own. AIEntities will
/// attack any enemies in their field of view.
/// TODO: later, they will be able to do things such as take cover if near obsticles,
/// use any nearby health sources if low on health, etc...
/// @author Abdullah Aghazadah
/// @date 4/18/16
class AIEntity: public DynamicEntity
{
    Q_OBJECT
public:
    // constants
    static const int CHECK_FOV_FREQUENCY_MS = 500; // how often (in ms) the AIEntity should check its field
                                                // of view
    static const int ATTACK_FREQUENCY_MS = 1500;   // how often (in ms) the AIEntity should be told to attack (swing)
                                                // when it is within strikin range of an enemy

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
