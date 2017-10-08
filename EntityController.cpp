#include "EntityController.h"

EntityController::EntityController(Entity *entityToControl) : entityControlled_(entityToControl)
{
    // Entity maintains a collection of EntityControllers that are operating on it.
    // When an Entity dies, it kills all of its controllers.
    // When we construct an EntityController we can pass in the entity that it will operate on via its ctor.
    // This ctor will need to ensure that it adds the object ('this') to the Entity's list of controllers (via Entity::addController())
    // so that the lifetime of the EntityController can be managed by the Entity (which is the policy we have chosen).
    entityToControl->addController(this);
}

/// Returns the entity that this EntityController is controlling.
Entity *EntityController::entityControlled()
{
    return entityControlled_;
}

Entity *EntityController::setEntityControlled(Entity* entity)
{
    entityControlled_ = entity;
}
