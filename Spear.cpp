#include "Spear.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug>

void Spear::resetVariables()
{
    headingBackward_ = false;
    headingForward_ = false;
    headingBackwardDueToCollision_ = false;
    alreadyThrusting_ = false;
}

Spear::Spear()
{
    // default performance
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    setThrustDistance(35);
    setThrustSpeed(250);

    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/spear.png");
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

//    setThrustDistance(200);
//    setThrustSpeed(500);
}

/// Tells the weapon to do a thrust.
///
/// The spear will thrust forward. It will thrust backward if it either hits
/// something or goes as far as it can.
void Spear::attack()
{
    // if its already thrusting, don't do anything
    if (alreadyThrusting_){
        return;
    }

    headingBackward_ = false;
    headingForward_ = true;
    currentThrustStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(thrustStep()));
    timer_->start(thrustStepFrequency_);
    alreadyThrusting_ = true;
}

/// Returns the point at which the weapon should be attached at.
///
/// This point is in local coordinates.
QPointF Spear::attachmentPoint()
{
    return attachmentPoint_;
}

/// Sets the attachment point to the specified point.
///
/// The attachment point is defined in local coordinates. It is the point that
/// tells us where the weapon should be be attached at.
void Spear::setAttachmentPoint(QPointF point)
{
    attachmentPoint_ = point;
}

/// Sets the tip of the spear, which will be used to do damage.
///
/// By default the tip of the spear is set as the length of the spear for the x,
/// and width of the spear/2 for the y. But depending on the drawing of the
/// spear you can set this to whatever you want.
void Spear::setTip(QPointF point)
{
    tip_ = point;
}

/// Resets the tip of the spear so that it is at x = length, and y = width/2.
void Spear::resetTip()
{
    QPointF pt;
    pt.setX(length());
    pt.setY(width()/2);
    setTip(pt);
}

/// Returns the position of the tip of the spear in local coordinates.
QPointF Spear::tip()
{
    return tip_;
}

/// Sets how long the spear is in pixels. Will move the tip appropriately.
void Spear::setLength(double length)
{
    pm_ = pm_.scaled(length,pm_.height());
    spr_->setPixmap(pm_);

    resetTip();

}

/// Returns the length of the spear in pixels.
double Spear::length()
{
    return sprite()->boundingRect().width();
}

/// Sets how wide the spear is in pixels. Will move the tip appropriately.
void Spear::setWidth(double width)
{
    pm_ = pm_.scaled(pm_.width(),width);
    spr_->setPixmap(pm_);

    resetTip();
}

/// Returns width of the spear in pixels.
double Spear::width()
{
    return sprite()->boundingRect().height();
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

/// Sets how far the spear thrusts (in pixels).
void Spear::setThrustDistance(double distance)
{
    // thrust distance is set by changing the maxThrustSteps_
    int numOfThrusts =  distance / thrustLengthEachStep_;
    maxThrustSteps_ = numOfThrusts;
    thrustDistance_ = thrustLengthEachStep_ * numOfThrusts;
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
    std::vector<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
    for (Entity* e: collidingEntities){
        if (e != this && e!= owner() && headingForward_){
            map()->removeEntity(e);
            delete e;
            headingBackwardDueToCollision_ = true;
            headingBackward_ = false;
            headingForward_ = false;
        }
    }

    // if heading backward due to collision, move backward
    if (headingBackwardDueToCollision_ && currentThrustStep_ > 0){
        // move spear backward at current angle
        QLineF line(pointPos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPointPos(newPt);

        currentThrustStep_--;
        return;
    }

    // if moving forward, move forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear forward at current angle
        QLineF line(pointPos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward, move backward
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear backward at current angle
        QLineF line(pointPos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPointPos(newPt);

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
