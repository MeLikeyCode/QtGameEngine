#include "ECRotater.h"
#include "Entity.h"
#include "Sprite.h"
#include <cassert>
#include <QLineF>
#include <QTimer>
#include "Utilities.h"

ECRotater::ECRotater(Entity &entity):
    stepSize_(1),
    entity_(&entity),
    rotationTimer_(new QTimer(this)),
    rotateRight_(false),
    targetAngle_(0)
{
    connect(rotationTimer_,&QTimer::timeout,this,&ECRotater::rotateStep_);
}

/// Rotate the entity until it faces the specified angle. Angle must be between
/// 0-360 inclusive. A specified angle of 0 degrees is right, 90 degrees is
/// down and so on (in other words, angle increases clockwise).
void ECRotater::rotateTowards(int angle)
{
    // make sure angle is between 0-360
    assert(angle >= 0);
    assert(angle <= 360);

    QLineF line(QPointF(0,0),QPointF(1,1));
    line.setAngle(-entity_->facingAngle());

    QLineF line2(QPointF(0,0),QPointF(1,1));
    line2.setAngle(-angle);

    double angleBWLines = line.angleTo(line2);

    if (angleBWLines < 180){
        // rotate left
        rotateLeft(angleBWLines);
    }
    else {
        // rotate right
        rotateRight(360-angleBWLines);
    }
}

/// Rotates the Entity until it faces the specified point. The Entity will
/// rotate in whatever direction is the closest.
void ECRotater::rotateTowards(const QPointF &point)
{
    QLineF line(entity_->pos(),point);
    int r = 360-line.angle();
    rotateTowards(r);
}

/// Rotates the entity the specified number of degrees to the left.
void ECRotater::rotateLeft(int numDegrees)
{
    // stop previous rotations
    stopRotating();

    // set targetAngle and direction
    targetAngle_ = entity_->facingAngle() - numDegrees;
    rotateRight_ = false;

    // start the timer
    rotationTimer_->start(secondsToMs(frequency(stepSize_,entity_->rotationSpeed())));
}

/// Rotates the entity the specified number of degrees to the right.
void ECRotater::rotateRight(int numDegrees)
{
    // stop previous rotations
    stopRotating();

    // set targetAngle and direction
    targetAngle_ = entity_->facingAngle() + numDegrees;
    rotateRight_ = true;

    // start the timer
    rotationTimer_->start(secondsToMs(frequency(stepSize_,entity_->rotationSpeed())));
}

/// Stop the entity's rotating.
void ECRotater::stopRotating()
{
    rotationTimer_->stop();
}

/// Returns true if the ECRotater is currently rotating its Entity. @note This
/// function only returns true if the *ECRotater* is the one rotating the
/// Entity, not if the Entity is being rotated due to itself or some other
/// object.
bool ECRotater::isRotating() const
{
    return rotationTimer_->isActive();
}

/// Sets how many degrees the controlled entity will rotate each time it rotates.
/// In other words, sets the "granularity" of rotation.
/// @see ECPathMover::setStepSize()
void ECRotater::setStepSize(double degrees)
{
    stepSize_ = degrees;
}

/// Returns how many degrees the controlled entity rotates each time it rotates.
double ECRotater::stepSize()
{
    return stepSize_;
}

void ECRotater::rotateStep_()
{
    // if the entity has been destroyed, do nothing
    if (entity_.isNull()){
        stopRotating();
        return;
    }

    // if it has reached its targetAngle, stop rotating
    if (abs(entity_->facingAngle() - targetAngle_) == 0 ){
        rotationTimer_->stop();
    }
    // other wise, rotate once towards targetAngle
    else {
        rotateTowardsTargetAngle_();
    }
}

void ECRotater::rotateTowardsTargetAngle_()
{
    // rotate right if rotateRight
    if (rotateRight_){
        entity_->setFacingAngle(entity_->facingAngle() + 1);
    }

    // other wise rotate left
    else {
        entity_->setFacingAngle(entity_->facingAngle() - 1);
    }
}
