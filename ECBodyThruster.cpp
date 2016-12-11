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
    connect(controllerChaseEnemies_,&ECChaseEnemies::entityChaseContinued,this,&ECBodyThruster::onEnemyChaseContinued);
    connect(controllerChaseEnemies_,&ECChaseEnemies::entityChaseStarted,this,&ECBodyThruster::onEnemyChaseContinued);
}

ECBodyThruster::~ECBodyThruster()
{
    delete controllerChaseEnemies_;
    delete bodyThrustAbility_;
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
