#include "Axe.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug>

Axe::Axe()
{
    // defaults
    swingAngleEachStep_ = 5;
    swingFrequency_ = 50;
    numOfSwingStepsBackward_ = 9;
    numOfSwingStepsForward_ = 8;
    numOfCurrentSwingSteps_ = 0;
    swingingForwardPhase_ = false;
    swingingOutPhase_ = false;
    alreadySwinging_ = false;
    headingBackwardPhase_ = false;

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

    // set initial state variables
    numOfCurrentSwingSteps_ = 0;
    swingingForwardPhase_ = false;
    headingBackwardPhase_ = false;
    swingingOutPhase_ = true;
    alreadySwinging_ = true;


    // connect to attackStep
    connect(timer_,SIGNAL(timeout()),this,SLOT(swingStep()));
    timer_->start(swingFrequency_);
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

void Axe::swingStep()
{
    // if in swinging out phase
    if (swingingOutPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsBackward_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        numOfCurrentSwingSteps_++;
        return;
    }

    // if done with swinging out phase
    if (swingingOutPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsBackward_){
        swingingOutPhase_ = false;
        swingingForwardPhase_ = true;
        numOfCurrentSwingSteps_ = 0;
    }

    // if swinging forward
    if (swingingForwardPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsForward_ + numOfSwingStepsBackward_){
        setFacingAngle(facingAngle()+swingAngleEachStep_);
        numOfCurrentSwingSteps_++;
        return;
    }

    // if done swinging forward
    if (swingingForwardPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsForward_ + numOfSwingStepsBackward_){
        swingingForwardPhase_ = false;
        headingBackwardPhase_ = true;
        numOfCurrentSwingSteps_ = 0;
        return;
    }

    // if heading backward
    if (headingBackwardPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsForward_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        numOfCurrentSwingSteps_++;
        return;
    }

    // if done heading backward
    if (headingBackwardPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsForward_){
        timer_->disconnect();
        alreadySwinging_ = false;
    }
}
