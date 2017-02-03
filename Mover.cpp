#include "Mover.h"
#include <cassert>

Mover::Mover(Entity* entity):
    entity_(entity),
    isMovingEntity_(false)
{
    // empty
}

/// Returns the Entity that the Mover moves.
Entity *Mover::entity()
{
    return entity_;
}

/// Sets the Entity that the Mover should move.
void Mover::setEntity(Entity *entity)
{
    assert(!isMovingEntity()); // cannot call setEntity() while Mover is already moving an Entity

    entity_ = entity;
}

/// Stops moving the entity.
void Mover::stopMovingEntity()
{
    isMovingEntity_ = false;    // update internal variable
    stopMovingEntity_();      // delegate to derived class's implementation
}

/// Returns true if the Mover is currently moving its Entity.
bool Mover::isMovingEntity()
{
    // if the isMoving_ flag is set and the entity isn't dead, were moving
    return isMovingEntity_ && !entity_.isNull();
}
