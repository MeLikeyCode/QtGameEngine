#include "ECBodyThruster.h"

#include "ECEnemyChaser.h"
#include "BodyThrust.h"
#include <cassert>

ECBodyThruster::ECBodyThruster(Entity *entity):
    EntityController(entity),
    controllerChaseEnemies_(new ECEnemyChaser(entity)),
    bodyThrustAbility_(new BodyThrust(*entity))
{
    // listen to chase controller
    connect(controllerChaseEnemies_,&ECEnemyChaser::entityChaseContinued,this,&ECBodyThruster::onEnemyChaseContinued);
    connect(controllerChaseEnemies_,&ECEnemyChaser::entityChaseStarted,this,&ECBodyThruster::onEnemyChaseContinued);
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
