#include "BodyThrust.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug> // TODO: remove
#include "Inventory.h"
#include "Entity.h"
#include "Sound.h"
#include <cassert>
#include "QTimer"

BodyThrust::BodyThrust(Entity &owner):
    NoTargetAbility(owner,nullptr)
{
    // default thrust parameters
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    setThrustDistance(45);
    setThrustSpeed(250);

    timer_ = new QTimer(this);
    resetVariables();

    // default damage
    damage_ = 5;

    soundEffect_ = new Sound("qrc:/resources/sounds/spear.wav");

    setIcon(new Sprite());
}

void BodyThrust::useImplementation()
{
    // assert pre conditions
    // before using the ability, make sure owner of ability is in a map
    Entity* theOwner = owner();
    assert(theOwner->map() != nullptr);  // make sure owner is in a map

    // set point that will be checked for collision
    double ownerWidth = theOwner->sprite()->size().width();
    double ownerHeight = theOwner->sprite()->size().height();
    collisionPoint_ = QPointF(ownerWidth/2,ownerHeight/2);

    // if its already thrusting, don't do anything
    if (alreadyThrusting_){
        return;
    }

    soundEffect_->play(1);

    headingBackward_ = false;
    headingForward_ = true;
    currentThrustStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(thrustStep_()));
    timer_->start(thrustStepFrequency_);
    alreadyThrusting_ = true;
}

/// Returns the number of pixels the owner will thrust.
double BodyThrust::thrustDistance()
{
    return thrustDistance_;
}

/// Returns the number of pixels per second the owner will thrust at.
double BodyThrust::thrustSpeed()
{
    return thrustSpeed_;
}

/// Sets how fast the owner thrusts at (in pixels pers second).
void BodyThrust::setThrustSpeed(double speed)
{
    // thrust speed is set by changing thrustFrequency_
    double thrustsPerSecond = speed / thrustLengthEachStep_;
    thrustStepFrequency_ = 1000*(1.0/thrustsPerSecond);
    thrustSpeed_ = thrustLengthEachStep_ * thrustsPerSecond;
}

/// Sets how far the owner thrusts (in pixels). Also sets the cast range accordingly.
void BodyThrust::setThrustDistance(double distance)
{
    // thrust distance is set by changing the maxThrustSteps_
    int numOfThrusts =  distance / thrustLengthEachStep_;
    maxThrustSteps_ = numOfThrusts;
    thrustDistance_ = thrustLengthEachStep_ * numOfThrusts;
}

void BodyThrust::thrustStep_()
{
    // if moved backward enough, stop moving
    if (headingBackward_ && currentThrustStep_ >= maxThrustSteps_){
        timer_->disconnect();

        resetVariables();
        return;
    }

    // if moved backward enough due to collision, stop
    if (headingBackwardDueToCollision_ && currentThrustStep_ == 0){
        timer_->disconnect();

        resetVariables();
        return;
    }

    Entity* theOwner = owner();

    // if still moving forward, kill things with tip, then move backward
    // due to collision
    std::unordered_set<Entity*> collidingEntities = theOwner->map()->entities(theOwner->mapToMap(collisionPoint_));
    for (Entity* e: collidingEntities){
        if (e != (Entity*)theOwner && e->parent() != (Entity*)theOwner && headingForward_){
            theOwner->damage(e,damage_);
            headingBackwardDueToCollision_ = true;
            headingBackward_ = false;
            headingForward_ = false;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentThrustStep_ > 0){
        // move owner backward at current angle
        QLineF line(theOwner->pointPos(),QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pointPos().x() + line.dx();
        double newY = theOwner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPointPos(newPt);

        currentThrustStep_--;
        return;
    }

    // if moving forward, move forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move owner forward at current angle
        QLineF line(theOwner->pointPos(),QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pointPos().x() + line.dx();
        double newY = theOwner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_){
        // move owner backward at current angle
        QLineF line(theOwner->pointPos(),QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pointPos().x() + line.dx();
        double newY = theOwner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moved forward enough, move backward
    if (headingForward_ && currentThrustStep_ >= maxThrustSteps_){
        headingForward_ = false;
        headingBackward_ = true;
        currentThrustStep_ = 0;

        return;
    }

}

void BodyThrust::resetVariables()
{
    // inititial state
    headingBackward_ = false;
    headingForward_ = false;
    headingBackwardDueToCollision_ = false;
    alreadyThrusting_ = false;
}
