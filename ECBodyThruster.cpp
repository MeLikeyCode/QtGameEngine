#include "ECBodyThruster.h"

#include "ECChaseEnemies.h"
#include "BodyThrust.h"
#include <cassert>

ECBodyThruster::ECBodyThruster(Entity& entity):
    entity_(&entity),
    controllerChaseEnemies_(new ECChaseEnemies(entity)),
    bodyThrustAbility_(new BodyThrust(entity))
{
    // listen to chase controller
    connect(controllerChaseEnemies_.get(),&ECChaseEnemies::entityChaseContinued,this,&ECBodyThruster::onEnemyChaseContinued);
    connect(controllerChaseEnemies_.get(),&ECChaseEnemies::entityChaseStarted,this,&ECBodyThruster::onEnemyChaseContinued);
}

/// Executed whenever the controlled enity moves closer to the chased entity.
/// Will see if close enough, if so, will BodyThrust chased entity.
void ECBodyThruster::onEnemyChaseContinued(Entity *entityChased, double distance)
{
    // if the entity is close enough to body thrust the chased entity, body thrust it
    if (distance < bodyThrustAbility_->thrustDistance() * 2){
        bodyThrustAbility_->useImplementation();
        emit thrusted(entityChased);
    }
}
