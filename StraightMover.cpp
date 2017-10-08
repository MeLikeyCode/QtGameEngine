#include "StraightMover.h"
#include <QTimer>
#include "Utilities.h"
#include <cassert>
#include <QLineF>

StraightMover::StraightMover(Entity *entity):
    ECMover(entity),
    speed_(entity->speed()),
    faceTarget_(true),
    moveTimer_(new QTimer(this)),
    stepSize_(25)
{
    // empty
}

/// Sets the speed at which the Entity should be moved at.
void StraightMover::setSpeed(int speed)
{
    speed_ = speed;
}

/// Returns the speed at which the Entity is moved at.
int StraightMover::speed()
{
    return speed_;
}

/// Sets how big each step of the controlled entity will be.
/// The larger the stepSize value, the more "jumpy" the movement will appear (but it
/// should be faster to execute).
void StraightMover::setStepSize(int stepSize)
{
    stepSize_ = stepSize;
}

/// See setStepSize().
int StraightMover::stepSize()
{
    return stepSize_;
}

/// Starts moving the controlled entity towards the specified position.
void StraightMover::moveEntity_(const QPointF& pos)
{
    Entity* theEntity = entity();

    stopMovingEntity(); // stop moving (just in case already moving)

    targetPos_ = pos;

    // store initial angle (so we know when the entity has past its target point)
    QLineF line(theEntity->pos(),pos);
    initialAngle_ = line.angle();

    // face target position (if option enabled)
    if (faceTarget()){
        theEntity->setFacingAngle(-1 * line.angle());
    }

    // start moving
    connect(moveTimer_,&QTimer::timeout,this,&StraightMover::onMoveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,speed_)));
}

/// If true is passed in, makes the controlled entity face the target position before
/// proceeding to move towards it. If false is passed in, the controlled entity
/// will move without changing its facing angle.
void StraightMover::setFaceTarget(bool tf)
{
    faceTarget_ = tf;
}

/// Returns true if the controlled entity will face the target position before moving
/// towards it.
/// @see setFaceTarget()
bool StraightMover::faceTarget()
{
    return faceTarget_;
}

/// Executed periodically to move the controlled entity one step closer towards its target
/// position.
void StraightMover::onMoveStep_()
{
    Entity* theEntity = entity();

    // if entity has died, stop moving
    if (theEntity == nullptr){
        stopMovingEntity();
        return;
    }

    // move
    QLineF line(theEntity->pos(),targetPos_);
    line.setLength(stepSize_);
    double newX = theEntity->x()+line.dx();
    double newY = theEntity->y()+line.dy();
    theEntity->setPos(QPointF(newX,newY));

    // if close enough, stop moving
    const double EPSILON = 50;
    if (distance(theEntity->pos(),targetPos_) < EPSILON){
        emit entitySuccesfullyMoved(this);
        stopMovingEntity();
        return;
    }

    // if past (due to large step size) stop moving
    if (qAbs(line.angle() - initialAngle_) > 100){ // if we haven't passed, diff in angles should be tiny, surely under 100
        stopMovingEntity();
        return;
    }

}

/// This function is executed when the MoveBehavior is asked to stop moving the entity.
void StraightMover::stopMovingEntity_()
{
    moveTimer_->disconnect();
}
