#ifndef PROJECTILEMOVEBEHAVIORSPINNING_H
#define PROJECTILEMOVEBEHAVIORSPINNING_H

#include "ProjectileMoveBehavior.h"
#include <QPointF>

class ProjectileMoveBehaviorSpinning : public ProjectileMoveBehavior
{
public:
    ProjectileMoveBehaviorSpinning(double range, QPointF target);

    virtual void onMoveStep();
private:
    QPointF target_;
    double distanceMoved_;
    double range_;

    bool moveAngleCalculated_;
    double moveAngle_;
    double rotation_;
};

#endif // PROJECTILEMOVEBEHAVIORSPINNING_H
