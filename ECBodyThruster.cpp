#include "ECBodyThruster.h"

#include "ECChaser.h"
#include "BodyThrust.h"
#include <cassert>

ECBodyThruster::ECBodyThruster(Entity *entity):
    EntityController(entity),
    controllerChaseEnemies_(new ECChaser(entity)),
    bodyThrustAbility_(new BodyThrust(*entity))
{
    // listen to chase controller
    connect(controllerChaseEnemies_,&ECChaser::entityChaseContinued,this,&ECBodyThruster::onEnemyChaseContinued);
    connect(controllerChaseEnemies_,&ECChaser::entityChaseStarted,this,&ECBodyThruster::onEnemyChaseContinued);
}

void ECBodyThruster::addTargetEntity(Entity *entity)
{
    controllerChaseEnemies_->addChasee(entity);
}

void ECBodyThruster::removeTargetEntity(Entity *entity)
{
    controllerChaseEnemies_->removeChasee(entity);
}

std::unordered_set<Entity *> ECBodyThruster::targetEntities() const
{
    return controllerChaseEnemies_->chasees();
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
