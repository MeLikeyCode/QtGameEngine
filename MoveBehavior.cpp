#include "MoveBehavior.h"
#include <cassert>

MoveBehavior::MoveBehavior(Entity* entity):
    entity_(entity),
    isMoving_(false)
{
    // empty
}

/// Returns the Entity that the MoveBehavior moves.
Entity *MoveBehavior::entity()
{
    return entity_;
}

/// Sets the Entity that the MoveBehavior should move.
void MoveBehavior::setEntity(Entity *entity)
{
    assert(!isMoving()); // cannot call setEntity() while MoveBehavior is already moving an Entity

    entity_ = entity;
}

/// Stops moving the entity.
void MoveBehavior::stopMoving()
{
    isMoving_ = false;    // update internal variable
    onStopMoving_();      // delegate to derived class's implementation
}

/// Returns true if the MoveBehavior is currently moving its Entity.
bool MoveBehavior::isMoving()
{
    // if the isMoving_ flag is set and the entity isn't dead, were moving
    return isMoving_ && !entity_.isNull();
}
