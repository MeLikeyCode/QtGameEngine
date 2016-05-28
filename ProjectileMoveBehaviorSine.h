#ifndef PROJECTILEMOVEBEHAVIORSINE_H
#define PROJECTILEMOVEBEHAVIORSINE_H

#include "ProjectileMoveBehavior.h"
#include <QLineF>

class Entity;

class ProjectileMoveBehaviorSine : public ProjectileMoveBehavior
{
public:
    ProjectileMoveBehaviorSine(double amplitude, double wavelength_, double range,
                               QPointF target);

    virtual void onMoveStep();
private:
    double amplitude_;
    double wavelength_;
    double range_;
    QPointF target_;

    double distanceMoved_;
    bool horMoveVecCalculated_;
    QLineF horMoveVector_;
};

#endif // PROJECTILEMOVEBEHAVIORSINE_H
