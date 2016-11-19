#include "BodyThrust.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug> // TODO: remove
#include "Inventory.h"
#include "DynamicEntity.h"
#include "Sound.h"
#include <cassert>

void BodyThrust::resetVariables()
{
    headingBackward_ = false;
    headingForward_ = false;
    headingBackwardDueToCollision_ = false;
    alreadyThrusting_ = false;
}

BodyThrust::BodyThrust()
{
    // default thrust parameters
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    setThrustDistance(45);
    setThrustSpeed(250);

    // default sprite
    Sprite* spr = new Sprite();
    setSprite(spr);

    // default tip (length and width/2)
    resetTip();

    // default attachment point
    QPointF pt;
    pt.setX(spr->currentFrame().width()/3);
    pt.setY(spr->currentFrame().height()/2);
    setAttachmentPoint(pt);

    timer_ = new QTimer(this);
    resetVariables();

//    setThrustDistance(200);
//    setThrustSpeed(500);

    // default damage
    damage_ = 5;

    soundEffect_ = new Sound("qrc:/resources/sounds/spear.wav");
}

/// Will thrust the spear forward. The "position" argument is ignored.
void BodyThrust::attack(QPointF position)
{
    // if its already thrusting, don't do anything
    if (alreadyThrusting_){
        return;
    }

    soundEffect_->play(1);

    headingBackward_ = false;
    headingForward_ = true;
    currentThrustStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(thrustStep()));
    timer_->start(thrustStepFrequency_);
    alreadyThrusting_ = true;
}

/// Returns the number of pixels the spear thrusts.
double BodyThrust::thrustDistance()
{
    return thrustDistance_;
}

/// Returns the number of pixels per second the spear thrusts at.
double BodyThrust::thrustSpeed()
{
    return thrustSpeed_;
}

/// Sets how fast the spear thrusts at (in pixels pers second).
void BodyThrust::setThrustSpeed(double speed)
{
    // thrust speed is set by changing thrustFrequency_
    double thrustsPerSecond = speed / thrustLengthEachStep_;
    thrustStepFrequency_ = 1000*(1.0/thrustsPerSecond);
    thrustSpeed_ = thrustLengthEachStep_ * thrustsPerSecond;
}

/// Sets how far the spear thrusts (in pixels). Also sets the cast range accordingly.
void BodyThrust::setThrustDistance(double distance)
{
    // thrust distance is set by changing the maxThrustSteps_
    int numOfThrusts =  distance / thrustLengthEachStep_;
    maxThrustSteps_ = numOfThrusts;
    thrustDistance_ = thrustLengthEachStep_ * numOfThrusts;

    setCastRange(thrustDistance_);
}

void BodyThrust::thrustStep()
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

    Inventory* inv = inventory();
    assert(inv);
    DynamicEntity* owner = inv->entity();
    setTip(QPointF(owner->sprite()->size().width()/2,owner->sprite()->size().height()/2));

    // if still moving forward, kill things with tip, then move backward
    // due to collision
    std::unordered_set<Entity*> collidingEntities = map()->entities(owner->mapToMap(tip()));
    Entity* theOwner = inventory()->entity();
    for (Entity* e: collidingEntities){
        if (e != this && e!= theOwner && e->parent() != theOwner && headingForward_){
            damage(e,damage_);
            headingBackwardDueToCollision_ = true;
            headingBackward_ = false;
            headingForward_ = false;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentThrustStep_ > 0){
        // move spear backward at current angle
        QLineF line(owner->pointPos(),QPointF(1,1));
        line.setAngle(360-owner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = owner->pointPos().x() + line.dx();
        double newY = owner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        owner->setPointPos(newPt);

        currentThrustStep_--;
        return;
    }

    // if moving forward, move forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear forward at current angle
        QLineF line(owner->pointPos(),QPointF(1,1));
        line.setAngle(360-owner->facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = owner->pointPos().x() + line.dx();
        double newY = owner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        owner->setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear backward at current angle
        QLineF line(owner->pointPos(),QPointF(1,1));
        line.setAngle(360-owner->facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = owner->pointPos().x() + line.dx();
        double newY = owner->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        owner->setPointPos(newPt);

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
