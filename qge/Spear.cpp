#include "Spear.h"

#include "Sprite.h"
#include "Map.h"
#include "Inventory.h"
#include "Entity.h"
#include "Sound.h"
#include "CBDamage.h"
#include "TopDownSprite.h"

using namespace qge;

void Spear::resetVariables()
{
    headingBackward_ = false;
    headingForward_ = false;
    headingBackwardDueToCollision_ = false;
    alreadyThrusting_ = false;
}

Spear::Spear()
{
    // set collision behavior
    setCollisionBehavior(new CBDamage(0,10));

    // default thrust parameters
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    setThrustDistance(45);
    setThrustSpeed(250);

    // default sprite
    EntitySprite* spr = new TopDownSprite(QPixmap(":resources/graphics/weapons/spear.png"));
    setSprite(spr);

    // default tip (length and width/2)
    resetTip();

    // default attachment point
    QPointF pt;
    pt.setX(spr->currentlyDisplayedFrame().width()/3);
    pt.setY(spr->currentlyDisplayedFrame().height()/2);
    setAttachmentPoint(pt);

    timer_ = new QTimer(this);
    resetVariables();

    soundEffect_ = new Sound("qrc:/resources/sounds/spear.wav", this);
}

/// Will thrust the spear forward. The "position" argument is ignored.
void Spear::attack(QPointF position)
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
double Spear::thrustDistance()
{
    return thrustDistance_;
}

/// Returns the number of pixels per second the spear thrusts at.
double Spear::thrustSpeed()
{
    return thrustSpeed_;
}

/// Sets how fast the spear thrusts at (in pixels pers second).
void Spear::setThrustSpeed(double speed)
{
    // thrust speed is set by changing thrustFrequency_
    double thrustsPerSecond = speed / thrustLengthEachStep_;
    thrustStepFrequency_ = 1000*(1.0/thrustsPerSecond);
    thrustSpeed_ = thrustLengthEachStep_ * thrustsPerSecond;
}

/// Sets how far the spear thrusts (in pixels). Also sets the cast range accordingly.
void Spear::setThrustDistance(double distance)
{
    // thrust distance is set by changing the maxThrustSteps_
    int numOfThrusts =  distance / thrustLengthEachStep_;
    maxThrustSteps_ = numOfThrusts;
    thrustDistance_ = thrustLengthEachStep_ * numOfThrusts;

    setCastRange(thrustDistance_);
}

void Spear::thrustStep()
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

    // if still moving forward, kill things with tip, then move backward
    // due to collision
    std::unordered_set<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
    Entity* theOwner = inventory()->entity();
    for (Entity* e: collidingEntities){
        if (e != this && e!= theOwner && e->parent() != theOwner && headingForward_){
            collisionBehavior()->onCollided(this,e,{},{}); // let collision behavior handle collision
            headingBackwardDueToCollision_ = true;
            headingBackward_ = false;
            headingForward_ = false;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentThrustStep_ > 0){
        // move spear backward at current angle
        QLineF line(pos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = pos().x() + line.dx();
        double newY = pos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPos(newPt);

        currentThrustStep_--;
        return;
    }

    // if moving forward, move forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear forward at current angle
        QLineF line(pos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = pos().x() + line.dx();
        double newY = pos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear backward at current angle
        QLineF line(pos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = pos().x() + line.dx();
        double newY = pos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPos(newPt);

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
