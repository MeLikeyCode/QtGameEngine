#include "ECBodyThruster.h"

#include "ECChaser.h"
#include "BodyThrust.h"
#include "QtUtilities.h"

using namespace qge;

ECBodyThruster::ECBodyThruster(Entity *entity):
    EntityController(entity),
    controllerChaseEnemies_(new ECChaser(entity)),
    bodyThrustAbility_(new BodyThrust(entity)),
    periodicCheckTimer_(new QTimer(this)),
    lastEntityChased_(nullptr)
{
    // set stop distance for chaser to thrust distance of body thrust ability
    controllerChaseEnemies_->setStopDistance(bodyThrustAbility_->thrustDistance());

    // listen to chase controller
    connect(controllerChaseEnemies_,&ECChaser::entityChaseContinued,this,&ECBodyThruster::onChaseContinued_);
    connect(controllerChaseEnemies_,&ECChaser::entityChaseStarted,this,&ECBodyThruster::onChaseContinued_);
    connect(controllerChaseEnemies_,&ECChaser::entityChasePaused,this,&ECBodyThruster::onChasePaused_);

    // listen to timer
    connect(periodicCheckTimer_,&QTimer::timeout,this,&ECBodyThruster::periodicCheck_);
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

/// Set the distance that the controlled entity will thrust from.
void ECBodyThruster::setThrustDistance(double distance)
{
    controllerChaseEnemies_->setStopDistance(distance);
    bodyThrustAbility_->setThrustDistance(distance);
}

/// Sets the animation to play while the controlled entity is thrusting forward.
/// Pass in "" if you don't want a special animation to be played.
void ECBodyThruster::setAnimationToPlayWhileThrusting(const std::string &animationName)
{
    bodyThrustAbility_->setAnimationToPlayWhileThrusting(animationName);
}

/// Determines whether the field of view of the controlled enity is shown or not.
void ECBodyThruster::setShowFOV(bool tf)
{
    controllerChaseEnemies_->setShowFOV(tf);
}

/// Executed whenever the controlled enity moves closer to the chased entity.
/// Will see if close enough, if so, will BodyThrust chased entity.
void ECBodyThruster::onChaseContinued_(Entity *entityChased, double distance)
{
    lastEntityChased_ = entityChased;
    periodicCheckTimer_->stop();
    bodyThrustIfCloseEnough_();
}

/// Executed when the controlled entity gets within the stop distance of chased entity.
/// We will periodically body thrust.
void ECBodyThruster::onChasePaused_(Entity *entity)
{
    lastEntityChased_ = entity;
    periodicCheckTimer_->start(1000);
    bodyThrustIfCloseEnough_();
}


void ECBodyThruster::periodicCheck_()
{
    bodyThrustIfCloseEnough_();
}

void ECBodyThruster::bodyThrustIfCloseEnough_()
{
    if (lastEntityChased_.isNull())
        return;

    double dist = QtUtils::distance(lastEntityChased_->pos(),entityControlled()->pos());
    if (dist < bodyThrustAbility_->thrustDistance() && !bodyThrustAbility_->thrusting()){
        bodyThrustAbility_->useImplementation();
        emit thrusted(lastEntityChased_);
    }
}
