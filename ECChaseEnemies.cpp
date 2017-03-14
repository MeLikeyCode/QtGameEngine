#include "ECChaseEnemies.h"
#include "ECFieldOfViewEmitter.h"
#include "PathMover.h"
#include <cassert>
#include <QTimer>
#include "Map.h"
#include "Utilities.h"
#include "Game.h"
#include <QDebug> // TODO: test remove

ECChaseEnemies::ECChaseEnemies(Entity &entity):
    stopDistance_(15),
    controlledEntity_(&entity),
    fovEmitter_(new ECFieldOfViewEmitter(entity)),
    pathMover_(new PathMover(&entity)),
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
    connect(fovEmitter_.get(),&ECFieldOfViewEmitter::entityEntersFOV,this,&ECChaseEnemies::onEntityEntersFOV_);
    connect(fovEmitter_.get(),&ECFieldOfViewEmitter::entityLeavesFOV,this,&ECChaseEnemies::onEntityLeavesFOV_);

    // listen to path mover
    connect(pathMover_.get(),&PathMover::moved,this,&ECChaseEnemies::onEntityMoved_);

    // listen to when the chasing entity dies
    connect(controlledEntity_,&QObject::destroyed,this,&ECChaseEnemies::onChasingEntityDies_);

    // listen to when chasing entity leaves map
    connect(controlledEntity_,&Entity::mapLeft,this, &ECChaseEnemies::onChasingEntityLeavesMap_);

    // connect timer
    connect(chaseTimer_,&QTimer::timeout,this,&ECChaseEnemies::chaseStep_);

    // set up path mover
    pathMover_->setAlwaysFaceTargetPosition(true);
    pathMover_->setEntity(&entity);
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
    // TODO: test remove
    qDebug() << "onEntityEntersFOF_ executed";

    // if the controlled entity isn't supposed to chase anything, do nothing
    if (!shouldChase_)
        return;

    // if the controlled entity already has a target entity, do nothing
    if (targetEntity_ != nullptr)
        return;

    // if the entering entity is not an enemy, do nothing
    if (controlledEntity_->isAnEnemyGroup(entity->group()) == false)
        return;

    // listen to when target entity dies (so we can stop chasing)
    targetEntity_ = entity;
    disconnect(0,&QObject::destroyed,this,&ECChaseEnemies::onChasedEntityDies_); // disconnect any previous signals connect to this slot
    connect(entity,&QObject::destroyed,this,&ECChaseEnemies::onChasedEntityDies_);

    // listen to when target entity leaves map (so we can stop chasing)
    disconnect(0,&Entity::mapLeft,this,&ECChaseEnemies::onChasedEntityLeavesMap_);
    connect(entity,&Entity::mapLeft,this,&ECChaseEnemies::onChasedEntityLeavesMap_);

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
    qDebug() << "onEntityLeavesFOV_ executed"; // TODO: remove, test

    // if leaving entity is target of controlled entity, unset as target
    if (entity == targetEntity_){
        targetEntity_ = nullptr;

        // stop listening to enter/leave range for leaving entity
        controlledEntity_->map()->game()->removeWatchedEntity(entity,controlledEntity_);
        chaseTimer_->stop();
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
    qDebug() << "onEntityEntersRange_ executed"; // TODO: remove test
    pathMover_->stopMovingEntity();
    paused_ = true;
}

/// Executed whenever the chased entity just leaves stop distance from controlled entity.
/// Will start chasing it again.
void ECChaseEnemies::onEntityLeavesRange_(Entity *watched, Entity *watching, double range)
{
    qDebug() << "onEntityLeavesRange_ executed"; // TODO: remove test
    paused_ = false;
}

/// Executed when the chasing entity dies.
/// Will stop chasing.
void ECChaseEnemies::onChasingEntityDies_(QObject *entity)
{
    stopChasing();
}

/// Executed when the chased entity dies.
/// Will stop chasing.
void ECChaseEnemies::onChasedEntityDies_(QObject *entity)
{
    stopChasing();
}

/// Executed when the controlled entity (chasing entity) leaves a map.
/// If it left to no map (i.e. map ptr is now nullptr), will stop chasing.
void ECChaseEnemies::onChasingEntityLeavesMap_(Entity *entity)
{
    if (entity->map() == nullptr)
        stopChasing();
}

/// Executed when the chased entity leaves a Map.
/// If it left to no map (i.e. map ptr is now nullptr), will stop chasing.
void ECChaseEnemies::onChasedEntityLeavesMap_(Entity *entity)
{
    if (entity->map() == nullptr)
        stopChasing();
}

/// Takes controlled entity one step closer to chase victim :P (if chasing something).
void ECChaseEnemies::chaseStep_()
{
    // if whats being chased has died, stop chasing
    // if the thing chasing has died, stop chasing
    if (targetEntity_.isNull() || controlledEntity_.isNull()){
        stopChasing();
        return;
    }

    // make sure entity and one being chased are in a map
    Map* entitysMap = controlledEntity_->map();
    Map* chaseVictimsMap = targetEntity_->map();
    assert(entitysMap != nullptr && chaseVictimsMap != nullptr);

    // make sure is supposed to be chasing right now
    assert(shouldChase_);

    // order to move towards chase victim :P
    if (!paused_)
        pathMover_->moveEntity(targetEntity_->pointPos());
}
