#ifndef BODYTHRUSTENTITY_H
#define BODYTHRUSTENTITY_H

#include <set>
#include "DynamicEntity.h"

class Weapon;
class BodyThrust;

/// An Entity that will attack nearby enemmies with the BodyThrust ability.
/// @author Abdullah
/// @date 11/21/16
class BodyThrustEntity: public DynamicEntity
{
    Q_OBJECT
public:
    // constants
    static const int CHECK_FOV_FREQUENCY_MS = 500;  // how often (in ms) the AIEntity should check its field
                                                    // of view
    static const int ATTACK_FREQUENCY_MS = 1500;    // how often (in ms) the AIEntity should be told to attack (BodyThrust)
                                                    // when it is within strikin range of an enemy

    BodyThrustEntity();
    ~BodyThrustEntity();

    void addEnemy(int groupID);
    bool isEnemy(int groupID);
    bool isEnemy(Entity* entity);
    void removeEnemy(int groupID);

public slots:
    void checkFov_();
    void startCheckingFov_();
    void attackIfInRange_();
    void onTargetDelete_();
private:
    QTimer* timerCheckFov_;
    QTimer* attackTimer_;

    Entity* targetEntity_;
    BodyThrust* bodyThrust_;

    std::set<int> enemyGroups_;
};

#endif // BODYTHRUSTENTITY_H
