#include "BodyThrust.h"

#include "Sprite.h"
#include "Map.h"
#include "Inventory.h"
#include "Entity.h"
#include "Sound.h"
#include "QTimer"
#include "EntitySprite.h"

using namespace qge;

BodyThrust::BodyThrust(Entity *owner):
    NoTargetAbility(owner)
{
    // default thrust parameters
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    setThrustDistance(65);
    setThrustSpeed(250);

    timer_ = new QTimer(this);
    resetVariables();

    // default damage
    damage_ = 5;

    soundEffect_ = new Sound("qrc:/resources/sounds/spear.wav");

    setOwner(owner);
}

void BodyThrust::useImplementation()
{
    // assert pre conditions
    // before using the ability, make sure owner of ability is in a map
    Entity* theOwner = owner();
    assert(theOwner->map() != nullptr);  // make sure owner is in a map

    // set point that will be checked for collision
    double ownerWidth = theOwner->sprite()->boundingBox().width();
    double ownerHeight = theOwner->sprite()->boundingBox().height();
    collisionPoint_ = QPointF(64,64);

    // if its already thrusting, don't do anything
    if (alreadyThrusting_){
        return;
    }

    soundEffect_->play(1);

    damaged_ = false;
    headingBackward_ = false;
    headingForward_ = true;
    currentThrustStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(thrustStep_()));
    timer_->start(thrustStepFrequency_);
    alreadyThrusting_ = true;

    if (animationToPlay_ != ""){
        lastAnim_ = owner()->sprite()->playingAnimation();
        owner()->sprite()->play(animationToPlay_,-1,10,0);
    }
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

/// Returns true if the ability is currently being used, false other wise.
bool BodyThrust::thrusting() const
{
    return alreadyThrusting_;
}

/// Sets the animation that should play when the owner is thrusting forward.
/// Pass in "" to have no animation play (leave w.e. animation the owner has at the time).
void BodyThrust::setAnimationToPlayWhileThrusting(const std::string &animation)
{
    animationToPlay_ = animation;
}

/// Executed periodically to take the entity one step closer to body thrusting.
void BodyThrust::thrustStep_()
{
    const int EXTRA_BACK_STEPS = 0;

    Entity* theOwner = owner(); // if the entity to thrust is dead, were done
    if (theOwner == nullptr){
        done_();
        return;
    }

    Map* ownersMap = theOwner->map(); // if the entity to thrust is no longer in a map, were done
    if (ownersMap == nullptr){
        done_();
        return;
    }

    // if moved backward enough, stop moving
    if (headingBackward_ && currentThrustStep_ >= maxThrustSteps_ + EXTRA_BACK_STEPS){
        done_();
        return;
    }

    // if moved backward enough due to collision, stop
    if (headingBackwardDueToCollision_ && currentThrustStep_ == 0){
        done_();
        return;
    }

    // if still moving forward, damage things in the way, (then move backward) <- don't do the move backward yet (over-inflated bboxes won't let this work properly)
    std::unordered_set<Entity*> collidingEntities = ownersMap->entities(theOwner->mapToMap(collisionPoint_));
    for (Entity* e: collidingEntities){
        if (e != theOwner && e->parent() != theOwner && headingForward_ && !damaged_){
            theOwner->damageEnemy(e,damage_);
            damaged_ = true;
//            headingBackwardDueToCollision_ = true;
//            headingBackward_ = false;
//            headingForward_ = false;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentThrustStep_ > 0){
        // move owner backward at current angle
        QLineF line(theOwner->pos(),theOwner->pos()+QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pos().x() + line.dx();
        double newY = theOwner->pos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPos(newPt);

        currentThrustStep_--;
        return;
    }

    // if moving forward, move forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move owner forward at current angle
        QLineF line(theOwner->pos(),theOwner->pos()+QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pos().x() + line.dx();
        double newY = theOwner->pos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_ + EXTRA_BACK_STEPS){
        // move owner backward at current angle
        QLineF line(theOwner->pos(),theOwner->pos()+QPointF(1,1));
        line.setAngle(360-theOwner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = theOwner->pos().x() + line.dx();
        double newY = theOwner->pos().y() + line.dy();
        QPointF newPt(newX,newY);
        theOwner->setPos(newPt);

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
    damaged_ = false;
}

void BodyThrust::done_()
{
    resetVariables();
    timer_->disconnect();

    if (animationToPlay_ != ""){
        if (!lastAnim_.isNone()){
            owner()->sprite()->play(lastAnim_.name(),lastAnim_.timesLeftToPlay(),lastAnim_.fps(),lastAnim_.currentFrame());
        }
    }
}
