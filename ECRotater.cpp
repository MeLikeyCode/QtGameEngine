#include "ECRotater.h"
#include "Entity.h"
#include "Sprite.h"
#include <cassert>
#include <QLineF>
#include <QTimer>

ECRotater::ECRotater(Entity *entity):
    entity_(entity),
    rotationFrequency_(10),
    rotationTimer_(new QTimer(this)),
    rotateRight_(false),
    targetAngle_(0)
{
    // make sure passed in entity is not nullptr
    assert(entity != nullptr);
}

/// Rotate the entity until it faces the specified angle.
/// Angle must be between 0-360 inclusive.
/// A specified angle of 0 degrees is right, 90 degrees is down and so on.
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
    QLineF line(entity_->pointPos(),point);
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
    connect(rotationTimer_,SIGNAL(timeout()),this,SLOT(rotateStep_()));
    rotationTimer_->start(rotationFrequency_);
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
    connect(rotationTimer_,SIGNAL(timeout()),this,SLOT(rotateStep_()));
    rotationTimer_->start(rotationFrequency_);
}

/// Stop the entity's rotating.
void ECRotater::stopRotating()
{
    rotationTimer_->disconnect();
}

/// Sets how fast the entity should be rotated.
/// The actual rotation will be close but not exactly what you pass in.
void ECRotater::setRotationSpeed(double degreesPerSecond)
{
    double degPerMS = degreesPerSecond / 1000.0;
    rotationFrequency_ = 1.0 / degPerMS;
    // TODO: double check and test this math
}

/// Returns the entity's rotation speed in degrees per second.
double ECRotater::rotationSpeed()
{
    return 1000.0 / rotationFrequency_;
    // TODO: double check and test this math
}

void ECRotater::rotateStep_()
{
    // if it has reached its targetAngle, stop rotating
    if (abs(entity_->facingAngle() - targetAngle_) == 0 ){
        rotationTimer_->disconnect();
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
        rotate1Right_();
    }

    // other wise rotate left
    else {
        rotate1Left_();
    }
}

/// Rotates the entity 1 degrees to the right.
void ECRotater::rotate1Right_()
{
    entity_->sprite()->setRotation(entity_->sprite()->rotation()+1);
}

/// Rotates the entity 1 degrees to the left.
void ECRotater::rotate1Left_()
{
    entity_->sprite()->setRotation(entity_->sprite()->rotation()-1);
}
