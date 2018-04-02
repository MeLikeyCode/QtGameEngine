#include "EntityController.h"

using namespace qge;

EntityController::EntityController(Entity *entityToControl) : entityControlled_(entityToControl)
{
    // Entity maintains a collection of EntityControllers that are operating on it.
    // When an Entity dies, it kills all of its controllers.
    setParent(entityToControl);

    // listen to death
    connect(entityToControl,&Entity::dying,this,&EntityController::onControlledEntityDying_);
}

EntityController::~EntityController()
{
    // empty
}

/// Returns the entity that this EntityController is controlling.
/// Returns nullptr if the EntityController isn't controlling an Entity (this can happen if the
/// entity controlled dies).
Entity *EntityController::entityControlled()
{
    return entityControlled_;
}

/// Set the entity that this EntityController should control.
void EntityController::setEntityControlled(Entity* entity)
{
    disconnect(entityControlled_,&Entity::dying,this,&EntityController::onControlledEntityDying_); // stop listening to death of old entity

    setParent(entity);
    entityControlled_ = entity;

    connect(entityControlled_,&Entity::dying,this,&EntityController::onControlledEntityDying_); // listen to death of new entity
}

/// Executed when the controlled entity is dying.
/// Default implementation does nothing.
/// This function exists so that sub classes can easily respond to the controlled entity dying.
void EntityController::onControlledEntityDying(Entity *controlledEntity)
{
    // default impl, do nothing
}

/// Executed when the controlled entity is dying.
/// Will call virtual onControlledEntityDying() function (to let sublcasses handled it - default implementation does nothing).
void EntityController::onControlledEntityDying_(Entity *sender)
{
    onControlledEntityDying(sender);
}
