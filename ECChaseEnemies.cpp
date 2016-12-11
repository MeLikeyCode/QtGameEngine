#include "ECChaseEnemies.h"
#include "ECFieldOfViewEmitter.h"
#include "ECPathMover.h"
#include <cassert>
#include <QTimer>
#include "Map.h"
#include "Utilities.h"
#include "Game.h"

ECChaseEnemies::ECChaseEnemies(Entity &entity):
    stopDistance_(250),
    controlledEntity_(&entity),
    fovEmitter_(new ECFieldOfViewEmitter(entity)),
    pathMover_(new ECPathMover(entity)),
    chaseTimer_(new QTimer(this)),
    shouldChase_(true),
    paused_(false),
    targetEntity_(nullptr)
{
    // make sure entity is in a map
    Map* entitysMap = entity.map();
    assert(entitysMap != nullptr);

    // make sure entity's map is in a game
    Game* entitysGame = entitysMap->game();
    assert(entitysGame != nullptr);

    // listen to game
    connect(entitysGame, &Game::watchedEntityEntersRange, this, &ECChaseEnemies::onEntityEntersRange_);
    connect(entitysGame, &Game::watchedEntityLeavesRange, this, &ECChaseEnemies::onEntityLeavesRange_);

    // listen to fov emitter
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityEntersFOV,this,&ECChaseEnemies::onEntityEntersFOV_);
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityLeavesFOV,this,&ECChaseEnemies::onEntityLeavesFOV_);

    // listen to path mover
    connect(pathMover_,&ECPathMover::moved,this,&ECChaseEnemies::onEntityMoved_);

    // connect timer
    connect(chaseTimer_,&QTimer::timeout,this,&ECChaseEnemies::chaseStep_);

    // make controlled entity always face target position
    pathMover_->setAlwaysFaceTargetPosition(true);
}

ECChaseEnemies::~ECChaseEnemies()
{
    delete fovEmitter_;
    delete pathMover_;
    // chaseTimer_ automatically deleted since child QObject of this QObject
}

/// Makes it so the controlled entity stops chasing enemy entities.
/// The controlled entity will then simply ignore any enemy entities that enter
/// its field of view.
/// If the controlled entity is currently chasing, it will stop.
void ECChaseEnemies::stopChasing()
{    
    // if currently chasing stop
    if (targetEntity_ != nullptr){
        targetEntity_ = nullptr;
        chaseTimer_->disconnect();
    }

    shouldChase_ = false;
}

/// Makes it so that the controlled entity will chase any enemy entites that
/// enter its field of view.
void ECChaseEnemies::startChasing()
{
    shouldChase_ = true;
}

/// Sets the stop distance.
/// See stopDistance() for more info.
void ECChaseEnemies::setStopDistance(double distance)
{
    stopDistance_ = distance;
}

/// Returns the distance that the controlled Entity will stop before the
/// positions of the chased entity.
double ECChaseEnemies::stopDistance()
{
    return stopDistance_;
}

/// Executed whenever an entity enters the fov of the controlled entity.
/// If the controlled entity doesn't already have a target entity, will set the newly
/// entering entity as the target entity.
void ECChaseEnemies::onEntityEntersFOV_(Entity *entity)
{
    // if the controlled entity isn't supposed to chase anything, do nothing
    if (!shouldChase_)
        return;

    // if the controlled entity already has a target entity, do nothing
    if (targetEntity_ != nullptr)
        return;

    // if the entering entity is not an enemy, do nothing
    if (controlledEntity_->isAnEnemyGroup(entity->group()) == false)
        return;

    // otherwise, set entering entity as the target entity
    targetEntity_ = entity;

    // listen to when the target entity enters/leaves stop distance
    controlledEntity_->map()->game()->addWatchedEntity(entity,controlledEntity_,stopDistance_);

    chaseStep_();
    chaseTimer_->start(2000); // TODO: store in a (modifiable) variable somewhere

    double distBW = distance(controlledEntity_->pointPos(),entity->pointPos());
    emit entityChaseStarted(entity, distBW);
}

/// Executed whenever an entity leaves the fov of the controlled entity.
/// Will unset the leaving entity as the target entity.
void ECChaseEnemies::onEntityLeavesFOV_(Entity *entity)
{
    // if leaving entity is target of controlled entity, unset as target
    if (entity == targetEntity_){
        targetEntity_ = nullptr;

        // stop listening to enter/leave range for leaving entity
        controlledEntity_->map()->game()->removeWatchedEntity(entity,controlledEntity_);
        chaseTimer_->disconnect();
    }
}

/// Executed whenever the controlled entity moves towards its chase target.
void ECChaseEnemies::onEntityMoved_()
{
    // do nothing if nothing being chased
    if (targetEntity_.isNull())
        return;

    double distBW = distance(controlledEntity_->pointPos(),targetEntity_->pointPos());
    emit entityChaseContinued(targetEntity_,distBW);
}

/// Executed whenever the controlled entity has just reached the stop distance from the chased entity.
/// Will stop moving towards the chased entity.
void ECChaseEnemies::onEntityEntersRange_(Entity *watched, Entity *watching, double range)
{
    pathMover_->stopMoving();
    paused_ = true;
}

/// Executed whenever the chased entity just leaves stop distance from controlled entity.
/// Will start chasing it again.
void ECChaseEnemies::onEntityLeavesRange_(Entity *watched, Entity *watching, double range)
{
    paused_ = false;
}

/// Takes controlled entity one step closer to chase victim :P (if chasing something).
void ECChaseEnemies::chaseStep_()
{
    // make sure is actually chasing something
    assert(!targetEntity_.isNull());

    // make sure entity and one being chased are in a map
    Map* entitysMap = controlledEntity_->map();
    Map* chaseVictimsMap = targetEntity_->map();
    assert(entitysMap != nullptr && chaseVictimsMap != nullptr);

    // make sure is supposed to be chasing right now
    assert(shouldChase_);

    // order to move towards chase victim :P
    if (!paused_)
        pathMover_->moveEntityTo(targetEntity_->pointPos());
}
