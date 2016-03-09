#ifndef PROJECTILEMOVEBEHAVIORHOMING_H
#define PROJECTILEMOVEBEHAVIORHOMING_H

#include "ProjectileMoveBehavior.h"

class Entity;

class ProjectileMoveBehaviorHoming: public ProjectileMoveBehavior
{
public:
    ProjectileMoveBehaviorHoming(Entity* entityToFollow);
    virtual double onMoveStep();
private:
    Entity* entityToFollow_;
};

#endif // PROJECTILEMOVEBEHAVIORHOMING_H
