#include "MoveBehavior.h"
#include <cassert>

MoveBehavior::MoveBehavior(Entity* entity):
    entity_(entity)
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
    entity_ = entity;
}
