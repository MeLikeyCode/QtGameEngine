#include "ECPathMover.h"
#include <QTimer>
#include <cassert>
#include <AsyncShortestPathFinder.h>
#include "Sprite.h"
#include "Map.h"
#include "ECRotater.h"
#include <QLineF>
#include "Utilities.h"

ECPathMover::ECPathMover(Entity &entity):
    alwaysFaceTargetPosition_(false),
    entity_(&entity),
    moveTimer_(new QTimer(this)),
    pf_(new AsyncShortestPathFinder()),
    rotater_(new ECRotater(entity)),
    stepSize_(5),
    pointsToFollow_(),
    targetPointIndex_(0),
    currentlyMoving_(false)
{
    // listen to when a path is calculated
    connect(pf_,SIGNAL(pathFound(std::vector<QPointF>)),this,SLOT(onPathCalculated_(std::vector<QPointF>)));
}

/// Moves the controlled entity to the specified pos on his map.
/// If the entity is already moving towards another point, it will stop, then
/// immediatley start moving towards this position.
void ECPathMover::moveEntityTo(const QPointF &pos)
{
    // make sure the entiy is in a map
    Map* entitysMap = entity_->map();
    assert(entitysMap != nullptr);

    // tell async path finder to start finding path to the pos,
    // when found, the path finder will emit an event (which we listen to)
    pf_->findPath(entitysMap->pathingMap(),entity_->pointPos(),pos);

    // set currently moving flag
    currentlyMoving_ = true;
}

/// Stops the controlled entity dead in its tracks.
void ECPathMover::stopMoving()
{
    moveTimer_->disconnect();   // disconnect timer
    pointsToFollow_.clear();    // reset variables
    targetPointIndex_ = 0;

    // play stand animation
    if (entity_->sprite()->hasAnimation("stand"))
        entity_->sprite()->play("stand",-1,100);

    // set moving flag
    currentlyMoving_ = false;
}

/// Returns true if the controlled entity is currently moving.
bool ECPathMover::entityIsCurrentlyMoving()
{
    return currentlyMoving_;
}

/// Returns true if the controlled entity will always face the target position while moving along
/// path.
/// See setAlwaysFaceTargetPosition() for more info.
bool ECPathMover::alwaysFaceTargetPosition()
{
    return alwaysFaceTargetPosition_;
}

/// If true is passed in, makes it so that as the controlled entity is moving, it will contineuosly
/// face it's target position. If false is passed in, the controlled entity will face the current
/// direction its heading in.
void ECPathMover::setAlwaysFaceTargetPosition(bool tf)
{
    alwaysFaceTargetPosition_ = tf;
}

/// Sets how many pixels the controlled entity should move every time he moves.
/// This in effect controlls the "granularity" of the movement.
/// Higher values means the controlled entity takes bigger steps but infrequently.
/// Lower values means the controlled entity takes frequent small steps.
/// Note that this does not effect the speed of the controlled entity, just the
/// movement "granularity"!
void ECPathMover::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// Returns how many pixels the controlled entity should move every time he moves.
/// @see setStepSize()
double ECPathMover::stepSize()
{
    return stepSize_;
}

/// Executed when the async path finder has succesfully calculated a requested path.
/// Will start the timer to make the entity move on the path.
void ECPathMover::onPathCalculated_(std::vector<QPointF> path)
{
    // stop/clear previous movement
    stopMoving();

    // set up variables for new path
    pointsToFollow_ = path;
    if (pointsToFollow_.size() > 1)
        targetPointIndex_ = 1; // start following the 1-eth point (0-eth causes initial backward movement)
    connect(moveTimer_,SIGNAL(timeout()),this,SLOT(moveStep_()));
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));

    // play walk animation
    if (entity_->sprite()->hasAnimation("walk"))
        entity_->sprite()->play("walk",-1,100);
}

/// Executed periodically to take the controlled entity one step along its path.
void ECPathMover::moveStep_()
{
    // if the entity is destroyed, disconnect
    if (entity_.isNull()){
        stopMoving();
        return;
    }

    // if the entity is not in a map, do nothing
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    // if there are no more points to follow and entity has reached its target
    // - stop moving
    if (targetPointIndex_ >= pointsToFollow_.size() - 1 && targetPointReached_()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        entity_->setCellPos(entitysMap->pathingMap().pointToCell(snapPt));

        stopMoving();

        return;
    }

    // if there are more points to follow and entity has reached its target
    // - snap to current target
    // - set next point as target
    // - face that point
    int pointsSize = pointsToFollow_.size() - 1;
    bool morePoints = targetPointIndex_ < pointsSize;
    if (morePoints && targetPointReached_()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        entity_->setCellPos(entitysMap->pathingMap().pointToCell(snapPt));

        // set next point as target
        ++targetPointIndex_;

        // face target
        if (!alwaysFaceTargetPosition_)
            rotater_->rotateTowards(pointsToFollow_[targetPointIndex_]);
    }

    // take a step closer towards the target (if there is a target)
    if (pointsToFollow_.size() > 0)
        stepTowardsTarget_();
}

/// Internal helper function that returns true if the controlled entity has reached
/// its current target point.
bool ECPathMover::targetPointReached_()
{
    // if there are no target points, return true
    if (pointsToFollow_.size() == 0)
        return true;

    // get a line b/w entity's pos and the targetPos
    QLineF ln(entity_->pointPos(),pointsToFollow_[targetPointIndex_]);

    // if the length of this line is less than a step size, return true
    return ln.length() < stepSize_;
}

/// Internal helper function that will cause the controlled entity to take one
/// step closer to its target point.
void ECPathMover::stepTowardsTarget_()
{
    // make sure points to follow is not empty
    assert(pointsToFollow_.size() != 0);

    // get a line b/w the entity's pos and the target pos
    QLineF ln(entity_->pointPos(),pointsToFollow_[targetPointIndex_]);

    // set the length of this line to be the same as stepSize
    ln.setLength(stepSize_);

    // find new pos
    double newX = entity_->pointPos().x() + ln.dx();
    double newY = entity_->pointPos().y() + ln.dy();
    QPointF newPt(newX,newY);

    if (alwaysFaceTargetPosition_)
        rotater_->rotateTowards(pointsToFollow_.back());

    entity_->setPointPos(newPt);
    emit moved(newPt);
}
