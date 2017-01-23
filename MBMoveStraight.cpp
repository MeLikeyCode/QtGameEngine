#include "MBMoveStraight.h"
#include <QTimer>
#include "Utilities.h"
#include <cassert>
#include <QLineF>

MBMoveStraight::MBMoveStraight(Entity *entity):
    MoveBehavior(entity),
    speed_(entity->speed()),
    faceTarget_(false),
    moveTimer_(new QTimer(this)),
    stepSize_(25)
{
    // empty
}

void MBMoveStraight::setSpeed(int speed)
{
    speed_ = speed;
}

int MBMoveStraight::speed()
{
    return speed_;
}

/// Sets how big each step of the controlled entity will be.
/// The larger the stepSize value, the more "jumpy" the movement will appear (but it
/// should be faster to execute).
void MBMoveStraight::setStepSize(int stepSize)
{
    stepSize_ = stepSize;
}

/// See setStepSize().
int MBMoveStraight::stepSize()
{
    return stepSize_;
}

/// Starts moving the controlle entity towards the specified position.
void MBMoveStraight::moveTo(const QPointF& pos)
{
    Entity* theEntity = entity();

    assert(theEntity != nullptr); // guard: make sure entity isn't dead

    stopMoving(); // stop moving (just in case already moving)

    targetPos_ = pos;

    // store initial angle (so we know when the entity has past its target point)
    QLineF line(theEntity->pointPos(),pos);
    initialAngle_ = line.angle();

    // face target position (if option enabled)
    if (faceTarget()){
        theEntity->setFacingAngle(-1 * line.angle());
    }

    // start moving
    connect(moveTimer_,&QTimer::timeout,this,&MBMoveStraight::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,speed_)));
}

/// If true is passed in, makes the controlled entity face the target position before
/// proceeding to move towards it. If false is passed in, the controlled entity
/// will move without changing its facing angle.
void MBMoveStraight::setFaceTarget(bool tf)
{
    faceTarget_ = tf;
}

/// Returns true if the controlled entity will face the target position before moving
/// towards it.
/// @see setFaceTarget()
bool MBMoveStraight::faceTarget()
{
    return faceTarget_;
}

/// Executed periodically to move the controlled entity one step closer towards its target
/// position.
void MBMoveStraight::moveStep_()
{
    Entity* theEntity = entity();

    // if controlled entity has died, stop moving
    if (theEntity == nullptr){
        stopMoving();
        return;
    }

    // move
    QLineF line(theEntity->pointPos(),targetPos_);
    line.setLength(stepSize_);
    double newX = theEntity->pointX()+line.dx();
    double newY = theEntity->pointY()+line.dy();
    theEntity->setPointPos(QPointF(newX,newY));

    // if close enough, stop moving
    const double EPSILON = 50;
    if (distance(theEntity->pointPos(),targetPos_) < EPSILON){
        stopMoving();
        return;
    }

    // if past (due to large step size) stop moving
    if (qAbs(line.angle() - initialAngle_) > 100){ // if we haven't passed, diff in angles should be tiny, surely under 100
        stopMoving();
        return;
    }

}

/// This function is executed when the MoveBehavior is asked to stop moving the entity.
/// Will disconnect moveTimer_.
void MBMoveStraight::onStopMoving_()
{
    moveTimer_->disconnect();
}
