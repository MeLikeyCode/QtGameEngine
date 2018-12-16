#include "ECPathMover.h"

#include "AsyncShortestPathFinder.h"
#include "Sprite.h"
#include "Map.h"
#include "ECRotater.h"
#include "Utilities.h"
#include "EntitySprite.h"

using namespace qge;

ECPathMover::ECPathMover(Entity *entity):
    ECMover(entity),
    alwaysFaceTargetPosition_(false),
    moveTimer_(new QTimer(this)),
    pf_(new AsyncShortestPathFinder()),
    rotater_(new ECRotater(entity)),
    stepSize_(5),
    pointsToFollow_(),
    targetPointIndex_(0),
    currentlyMoving_(false)
{
    // listen to when a path is calculated
    connect(pf_.get(),SIGNAL(pathFound(std::vector<QPointF>)),this,SLOT(onPathCalculated_(std::vector<QPointF>)));
}

/// Returns true if the entity will always face the target position while
/// moving along path or simply face the current direction in which its
/// heading. See setAlwaysFaceTargetPosition() for more info.
bool ECPathMover::alwaysFaceTargetPosition()
{
    return alwaysFaceTargetPosition_;
}

/// If true is passed in, makes it so that as the entity is moving, it will
/// contineuosly face it's target position. If false is passed in, the
/// controlled entity will face the current direction its heading in.
void ECPathMover::setAlwaysFaceTargetPosition(bool tf)
{
    alwaysFaceTargetPosition_ = tf;
}

/// Sets how many pixels the entity should move every time he moves.
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
    stopMovingEntity();

    // if entity to move is dead by now, were done
    Entity* ent = entity();
    if (ent == nullptr)
        return;

    // if entity tried to move from same cell to same cell, do nothing
    if (path.size() == 0 || path.size() == 1)
        return;

     path.pop_back();

    // set up variables for new path
    pointsToFollow_ = path;
    if (pointsToFollow_.size() > 1)
        targetPointIndex_ = 1; // start following the 1-eth point (0-eth causes initial backward movement)
    connect(moveTimer_,SIGNAL(timeout()),this,SLOT(onMoveStep_()));
    moveTimer_->start(secondsToMs(frequency(stepSize_,ent->speed())));

    // play walk animation (if controlled entity has one)
    EntitySprite* entitysSprite = ent->sprite();
    if (entitysSprite != nullptr)
        if (entitysSprite->hasAnimation("walk"))
            ent->sprite()->play("walk",-1,10,0);
}

/// Executed periodically to take the controlled entity one step along its path.
void ECPathMover::onMoveStep_()
{
    // if the entity is destroyed, disconnect
    Entity* ent = entity();
    if (ent == nullptr){
        stopMovingEntity();
        return;
    }

    // if the entity is not in a map, do nothing
    Map* entitysMap = ent->map();
    if (entitysMap == nullptr)
        return;

    // if there are no more points to follow and entity has reached its target
    // - stop moving
    if (targetPointIndex_ >= pointsToFollow_.size() - 1 && targetPointReached_()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        ent->setCellPos(entitysMap->pathingMap().pointToCell(snapPt));

        stopMovingEntity();

        return;
    }

    // if there are more points to follow and entity has reached its target
    // - snap to current target
    // - set next point as target
    // - face that point
    int pointsSize = pointsToFollow_.size();
    bool morePoints = targetPointIndex_ < pointsSize;
    if (morePoints && targetPointReached_()){
        // snap
        QPointF snapPt = pointsToFollow_[targetPointIndex_];
        ent->setCellPos(entitysMap->pathingMap().pointToCell(snapPt));

        // set next point as target
        ++targetPointIndex_;

        // face target
        if (!alwaysFaceTargetPosition_)
            rotater_->rotateTowards(pointsToFollow_[targetPointIndex_]);
    }

    stepTowardsTarget_();
}

/// Starts moving the entity towards the specified position.
void ECPathMover::moveEntity_(const QPointF &toPos)
{
    // make sure the entiy is in a map
    Map* entitysMap = entity()->map();
    assert(entitysMap != nullptr);

    // tell async path finder to start finding path to the pos,
    // when found, the path finder will emit an event (which we listen to)
    pf_->findPath(entitysMap->pathingMap(),entity()->pos(),toPos);
}

/// This function is executed when the MoveBehavior is asked to stop moving the entity.
void ECPathMover::stopMovingEntity_()
{
    moveTimer_->disconnect();   // disconnect timer
    pointsToFollow_.clear();    // reset variables
    targetPointIndex_ = 0;

    // if controlled entity is dead
    Entity* ent = entity();
    if (ent == nullptr)
        return; // were done

    // otherwise, play stand animation (if controlled entity has one)
    EntitySprite* entitysSprite = ent->sprite();
    if (entitysSprite != nullptr)
        if (entitysSprite->hasAnimation("stand"))
            ent->sprite()->play("stand",-1,10,0);
}

/// Internal helper function that returns true if the controlled entity has reached
/// its current target point.
bool ECPathMover::targetPointReached_()
{
    // if there are no target points, return true
    if (pointsToFollow_.size() == 0)
        return true;

    // get a line b/w entity's pos and the targetPos
    QLineF ln(entity()->pos(),pointsToFollow_[targetPointIndex_]);

    // if the length of this line is less than a step size, return true
    return ln.length() < stepSize_;
}

/// Internal helper function that will cause the controlled entity to take one
/// step closer to its target point.
void ECPathMover::stepTowardsTarget_()
{
    // do nothing if there are no points to follow
    if (pointsToFollow_.size() <= 0)
        return;

    Entity* ent = entity();

    // get a line b/w the entity's pos and the target pos
    QLineF ln(ent->pos(),pointsToFollow_[targetPointIndex_]);

    // set the length of this line to be the same as stepSize
    ln.setLength(stepSize_);

    // find new pos
    double newX = ent->pos().x() + ln.dx();
    double newY = ent->pos().y() + ln.dy();
    QPointF newPt(newX,newY);

    if (alwaysFaceTargetPosition_)
        rotater_->rotateTowards(pointsToFollow_.back());

    ent->setPos(newPt);
    emit moved(newPt);
}
