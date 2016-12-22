#ifndef PROJECTILEMOVEBEHAVIORHOMING_H
#define PROJECTILEMOVEBEHAVIORHOMING_H

#include "ProjectileMoveBehavior.h"
#include "Entity.h"
#include <QPointer>

class ProjectileMoveBehaviorHoming: public ProjectileMoveBehavior
{
public:
    ProjectileMoveBehaviorHoming(Entity* entityToFollow);
    virtual void onMoveStep();
private:
    QPointer<Entity> entityToFollow_;
};

#endif // PROJECTILEMOVEBEHAVIORHOMING_H
