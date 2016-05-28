#ifndef PROJECTILEMOVEBEHAVIORSTRAIGHT_H
#define PROJECTILEMOVEBEHAVIORSTRAIGHT_H

#include "ProjectileMoveBehavior.h"
#include <QPointF>

class ProjectileMoveBehaviorStraight : public ProjectileMoveBehavior
{
public:
    ProjectileMoveBehaviorStraight(double range, QPointF target);

    virtual void onMoveStep();
private:
    double range_;
    QPointF target_;
    double distanceMoved_;

    bool moveAngleCalculated_;
    double moveAngle_;
};

#endif // PROJECTILEMOVEBEHAVIORSTRAIGHT_H
