#ifndef PROJECTILEMOVEBEHAVIORSINE_H
#define PROJECTILEMOVEBEHAVIORSINE_H

#include "ProjectileMoveBehavior.h"
#include <QPointF>

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

    bool startPosStored_;
    QPointF start_;
    double xPos_; // current x position in the *sine graph* (not map)
};

#endif // PROJECTILEMOVEBEHAVIORSINE_H
