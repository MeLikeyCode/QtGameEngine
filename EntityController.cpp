#include "EntityController.h"

EntityController::EntityController(Entity *entityToControl) : entityControlled_(entityToControl)
{
    // Entity maintains a collection of EntityControllers that are operating on it.
    // When an Entity dies, it kills all of its controllers.
    setParent(entityToControl);
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
    setParent(entity);
    entityControlled_ = entity;
}
