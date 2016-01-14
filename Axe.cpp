#include "Axe.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug>

void Axe::resetVariables()
{
    headingBackward_ = false;
    swingingForward_ = false;
    headingBackwardDueToCollision_ = false;
    alreadySwinging_ = false;
    swingingOut_ = true;
}

Axe::Axe()
{
    // default performance
    currentSwingStep_ = 0;
    swingAngleEachStep_ = 5;
    swingStepFrequency_ = 25; // TODO replace with setSwingSpeed()
    maxSwingSteps_ = 10; // TODO replace with setTotalSwingAngle()
    maxSwingOutSteps_ = 5;

    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/axe.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // default tip (length and width/2)
    resetTip();

    // default attachment point
    QPointF pt;
    pt.setX(pm_.width()/3);
    pt.setY(pm_.height()/2);
    setAttachmentPoint(pt);

    timer_ = new QTimer(this);
    resetVariables();
}

/// Tells the weapon to do a thrust.
///
/// The Axe will swing.
void Axe::attack()
{
    // if its already thrusting, don't do anything
    if (alreadySwinging_){
        return;
    }

    headingBackward_ = false;
    swingingForward_ = true;
    currentSwingStep_ = 0;
    currentSwingOutStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(swingStep()));
    timer_->start(swingStepFrequency_);
    alreadySwinging_ = true;
}

/// Returns the point at which the weapon should be attached at.
///
/// This point is in local coordinates.
QPointF Axe::attachmentPoint()
{
    return attachmentPoint_;
}

/// Sets the attachment point to the specified point.
///
/// The attachment point is defined in local coordinates. It is the point that
/// tells us where the weapon should be be attached at.
void Axe::setAttachmentPoint(QPointF point)
{
    attachmentPoint_ = point;
}

/// Sets the tip of the Axe, which will be used to do damage.
///
/// By default the tip of the Axe is set as the length of the Axe for the x,
/// and width of the Axe/2 for the y. But depending on the drawing of the
/// Axe you can set this to whatever you want.
void Axe::setTip(QPointF point)
{
    tip_ = point;
}

/// Resets the tip of the Axe so that it is at x = length, and y = width/2.
void Axe::resetTip()
{
    QPointF pt;
    pt.setX(length());
    pt.setY(width()/2);
    setTip(pt);
}

/// Returns the position of the tip of the Axe in local coordinates.
QPointF Axe::tip()
{
    return tip_;
}

/// Sets how long the Axe is in pixels. Will move the tip appropriately.
void Axe::setLength(double length)
{
    pm_ = pm_.scaled(length,pm_.height());
    spr_->setPixmap(pm_);

    resetTip();

}

/// Returns the length of the Axe in pixels.
double Axe::length()
{
    return sprite()->boundingRect().width();
}

/// Sets how wide the Axe is in pixels. Will move the tip appropriately.
void Axe::setWidth(double width)
{
    pm_ = pm_.scaled(pm_.width(),width);
    spr_->setPixmap(pm_);

    resetTip();
}

/// Returns width of the Axe in pixels.
double Axe::width()
{
    return sprite()->boundingRect().height();
}

/// Returns the number of degrees the Axe thrusts.
double Axe::totalSwingAgnel()
{
    return totalSwingAngle_;
}

/// Returns the number of pixels per second the Axe thrusts at.
double Axe::swingSpeed()
{
    return swingSpeed_;
}

/// Sets how fast the Axe thrusts at (in pixels pers second).
void Axe::setSwingingSpeed(double speed)
{
    // thrust speed is set by changing thrustFrequency_
    double thrustsPerSecond = speed / swingAngleEachStep_;
    swingStepFrequency_ = 1000*(1.0/thrustsPerSecond);
    swingSpeed_ = swingAngleEachStep_ * thrustsPerSecond;
}

/// Sets how far the Axe thrusts (in pixels).
void Axe::setTotalSwingAngle(double distance)
{
    // thrust distance is set by changing the maxThrustSteps_
    int numOfThrusts =  distance / swingAngleEachStep_;
    maxSwingSteps_ = numOfThrusts;
    totalSwingAngle_ = swingAngleEachStep_ * numOfThrusts;
}

void Axe::swingStep()
{
    // if swinging out, move backward
    if (swingingOut_ && currentSwingOutStep_ < maxSwingOutSteps_){
        // rotate axe backward
        setFacingAngle(facingAngle() - swingAngleEachStep_);
        currentSwingOutStep_++;
        return;
    }

    // if enough swinging out
    if (currentSwingOutStep_ >= maxSwingOutSteps_){
        swingingOut_ = false;
    }

    // if moved backward enough, stop moving
    if (headingBackward_ && currentSwingStep_ >= maxSwingSteps_){
        timer_->disconnect();

        resetVariables();
        return;
    }

    // if moved backward enough due to collision, stop
    if (headingBackwardDueToCollision_ && currentSwingStep_ == 0){
        timer_->disconnect();

        resetVariables();
        return;
    }

    // if still moving forward, kill things with tip, then move backward
    // due to collision
    std::vector<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
    for (Entity* e: collidingEntities){
        if (e != this && e!= owner() && swingingForward_){
            map()->removeEntity(e);
            delete e;
            headingBackwardDueToCollision_ = true;
            headingBackward_ = false;
            swingingForward_ = false;
            return;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentSwingStep_ > 0){

        // rotate axe backward
        setFacingAngle(facingAngle() - swingAngleEachStep_);
        currentSwingStep_--;
        return;
    }

    // if moving forward, move forward
    if (swingingForward_ && currentSwingStep_ < maxSwingSteps_){


        // rotate axe forward
        setFacingAngle(facingAngle() + swingAngleEachStep_);
        currentSwingStep_++;
        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentSwingStep_ < maxSwingSteps_){

        // rotate axe
        setFacingAngle(facingAngle() - swingAngleEachStep_);
        currentSwingStep_++;
        return;
    }

    // if moved forward enough, move backward
    if (swingingForward_ && currentSwingStep_ >= maxSwingSteps_){
        swingingForward_ = false;
        headingBackward_ = true;
        currentSwingStep_ = 0;

        return;
    }

}
