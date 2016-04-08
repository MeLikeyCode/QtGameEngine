#ifndef MOVEBEHAVIORPATHFINDING_H
#define MOVEBEHAVIORPATHFINDING_H

#include "MoveBehavior.h"

class DynamicEntity;

/// Represents a movement behavior that allows the DynamicEntity to move to a
/// certain location while avoiding obsticles along the way ("path finding").
class MoveBehaviorPathFinding : public MoveBehavior
{
    Q_OBJECT
public:
    MoveBehaviorPathFinding(DynamicEntity* entity);
public slots:
    virtual void moveStep();
};

#endif // MOVEBEHAVIORPATHFINDING_H
