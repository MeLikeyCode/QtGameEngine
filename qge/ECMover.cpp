#include "ECMover.h"

using namespace qge;

/// Constructs a Mover that can move the specified Entity.
ECMover::ECMover(Entity* entity):
    EntityController(entity),
    isMovingEntity_(false)
{
    setEntity(entity);
}

/// Returns the Entity that the Mover moves.
Entity *ECMover::entity()
{
    return entityControlled();
}

/// Sets the Entity that the Mover should move.
void ECMover::setEntity(Entity *entity)
{
    assert(!isMovingEntity()); // cannot call setEntity() while Mover is already moving an Entity

    // listen to when the entity to move has died, so we can stop moving it
    disconnect(0,&Entity::dying,this,&ECMover::onEntityDied_); // stop listening to any previous entitys dieing
    connect(entity,&Entity::dying,this,&ECMover::onEntityDied_); // listen to when this entity dies

    setEntityControlled(entity);
}

/// Moves the entity from its current position to the specified position. Make
/// sure the entity is set before calling this funtion or an assertion will be
/// thrown. Some concrete Movers may move the entity straight, some may move it
/// in a sine motion, some may move it while spinning it, etc...
void ECMover::moveEntity(const QPointF &toPos)
{
    assert(entityControlled() != nullptr);  // assert
    isMovingEntity_ = true;
    moveEntity_(toPos);         // delegate
}

/// Tells the Mover to stops moving the entity.
void ECMover::stopMovingEntity()
{
    isMovingEntity_ = false;    // update internal variable
    stopMovingEntity_();      // delegate to derived class's implementation
}

/// Executed when the Entity that the Mover is supposed to move has died.
/// Will stop moving.
void ECMover::onEntityDied_(Entity *entity)
{
    stopMovingEntity();
}

/// Returns true if the Mover is currently moving its Entity.
bool ECMover::isMovingEntity()
{
    // if the isMoving_ flag is set and the entity isn't dead, were moving
    return isMovingEntity_ && entityControlled() != nullptr;
}
