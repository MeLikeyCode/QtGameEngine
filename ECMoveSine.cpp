#include "ECMoveSine.h"
#include <QTimer>
#include "Utilities.h"
#include <cassert>
#include <QtMath>

ECMoveSine::ECMoveSine(Entity &entity):
    entity_(&entity),
    moveTimer_(new QTimer(this)),
    amplitude_(10),
    wavelength_(100),
    targetPos_(),
    startPos_(),
    stepSize_(1),
    speed_(15),
    range_(0),
    faceTarget_(true),
    isMoving_(false),
    distanceMoved_(0)
{
    // empty
}

void ECMoveSine::moveTowards(QPointF pos)
{
    // guards
    assert(!entity_.isNull()); // make sure entity isn't dead
    Map* entitysMap = entity_->map();
    assert(entitysMap != nullptr); // make sure entity is in a map

    // stop moving if already moving
    stopMoving();

    // update internal variables
    isMoving_ = true;
    targetPos_ = pos;
    startPos_ = entity_->pointPos();

    // calculate range (so we know when we have gone past the target)
    QLineF line(entity_->pointPos(),pos);
    range_ = line.length();

    // face target position (if option enabled)
    if (faceTarget_){
        entity_->setFacingAngle(-1 * line.angle());
    }

    // start moving
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveSine::onMoveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,speed_)));
}

void ECMoveSine::stopMoving()
{
    moveTimer_->disconnect();
    isMoving_ = false;
    distanceMoved_ = 0;
}

/// Executed periodically to move the controlled entity to the next step of its movement.
void ECMoveSine::onMoveStep_()
{
    distanceMoved_ += stepSize_;

    QLineF startToTargetLine(startPos_,targetPos_);

    // calculate horizontal component
    QLineF hComponent(startToTargetLine);
    hComponent.setLength(distanceMoved_);

    // calculate vertical component
    // determine y offset at current xPos
    // y(x) = Acos(kx), A = amplitude, k = 2pi/wavelength
    double PI = 3.14159;
    double yOffset = amplitude_ * qSin(2*PI*distanceMoved_/wavelength_);

    QLineF vComponent(hComponent.p2(),QPointF(0,0));
    vComponent.setAngle(hComponent.angle());
    vComponent.setLength(qAbs(yOffset));

    if (yOffset > 0)
        vComponent.setAngle(vComponent.angle()+90);
    else
        vComponent.setAngle(vComponent.angle()-90);

    // move by horizontal component
    entity_->setPointX(entity_->pointX()+hComponent.dx());
    entity_->setPointY(entity_->pointY()+hComponent.dy());

    // move by vertical component
    entity_->setPointX(entity_->pointX()+vComponent.dx());
    entity_->setPointY(entity_->pointY()+vComponent.dy());

    // if moved far enough
    if (distanceMoved_ > range_){
        stopMoving();
    }
}
