#include "ECChaser.h"
#include "ECFieldOfViewEmitter.h"
#include "ECPathMover.h"
#include <cassert>
#include <QTimer>
#include "Map.h"
#include "Utilities.h"
#include "Game.h"
#include <QDebug> // TODO: test remove

ECChaser::ECChaser(Entity* entity):
    EntityController(entity),
    stopDistance_(100),
    fovEmitter_(new ECFieldOfViewEmitter(entity)),
    pathMover_(new ECPathMover(entity)),
    chaseTimer_(new QTimer(this)),
    paused_(false),
    targetEntity_(nullptr)
{
    // make sure entity is in a map
    Map* entitysMap = entity->map();
    assert(entitysMap != nullptr);

    // make sure entity's map is in a game
    Game* entitysGame = entitysMap->game();
    assert(entitysGame != nullptr);

    // listen to game
    connect(entitysGame, &Game::watchedEntityEntersRange, this, &ECChaser::onEntityEntersRange_);
    connect(entitysGame, &Game::watchedEntityLeavesRange, this, &ECChaser::onEntityLeavesRange_);

    // listen to fov emitter
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityEntersFOV,this,&ECChaser::onEntityEntersFOV_);
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityLeavesFOV,this,&ECChaser::onEntityLeavesFOV_);

    // listen to path mover
    connect(pathMover_,&ECPathMover::moved,this,&ECChaser::onEntityMoved_);

    // listen to when the controlled entity dies
    connect(entityControlled(),&QObject::destroyed,this,&ECChaser::onChasingEntityDies_);

    // listen to when controlled entity leaves map
    connect(entityControlled(),&Entity::mapLeft,this, &ECChaser::onChasingEntityLeavesMap_);

    // connect timer
    connect(chaseTimer_,&QTimer::timeout,this,&ECChaser::chaseStep_);

    // set up path mover
    pathMover_->setAlwaysFaceTargetPosition(true);
    pathMover_->setEntity(entity);
}

/// Adds an Entity to the list of Entities that the controlled Entity should chase.
void ECChaser::addChasee(Entity *entity)
{
    chasees_.insert(entity);
    connect(entity,&QObject::destroyed,this,&ECChaser::onChaseeDestroyed_);
}

/// Removes an Entity from the list of Entities that the controlled Entity should chase.
void ECChaser::removeChasee(Entity *entity)
{
    chasees_.erase(entity);
    disconnect(entity,&QObject::destroyed,this,&ECChaser::onChaseeDestroyed_);
}

/// Returns a list of the Entities that the controlled Entity should chase.
std::unordered_set<Entity *> ECChaser::chasees() const
{
    return chasees_;
}

/// Makes it so the controlled entity stops chasing enemy entities.
/// The controlled entity will then simply ignore any enemy chasees that enter
/// its field of view.
/// If the controlled entity is currently chasing, it will stop.
void ECChaser::stopChasing()
{    
    // if currently chasing stop
    if (targetEntity_ != nullptr){
        targetEntity_ = nullptr;
        disconnectFromTargetSignals_(); // stop listening to target signals
        chaseTimer_->stop(); // stop moving
    }

    fovEmitter_->turnOff(); // stop fov emitter
}

/// Makes it so that the controlled entity will chase any chasee entites that
/// enter its field of view.
void ECChaser::startChasing()
{
    fovEmitter_->turnOn();
}

/// Sets the stop distance.
/// See stopDistance() for more info.
void ECChaser::setStopDistance(double distance)
{
    stopDistance_ = distance;
}

/// Returns the distance that the controlled Entity will stop before the
/// positions of the chased entity.
double ECChaser::stopDistance() const
{
    return stopDistance_;
}

/// Executed whenever an entity enters the fov of the controlled entity.
/// If the controlled entity doesn't already have a target entity, will set the newly
/// entering entity as the target entity.
void ECChaser::onEntityEntersFOV_(Entity *entity)
{
    // TODO: test remove
    qDebug() << "onEntityEntersFOF_ executed";

    // if the controlled entity already has a target entity, do nothing
    if (targetEntity_ != nullptr)
        return;

    // if the entering entity is not a chasee, do nothing
    if (chasees_.find(entity) == std::end(chasees_))
        return;

    // set entering entity as target entity and connect to some signals from it
    targetEntity_ = entity;
    connectToTargetSignals_();

    chaseStep_();
    chaseTimer_->start(2000); // TODO: store in a (modifiable) variable somewhere

    double distBW = distance(entityControlled()->pos(),entity->pos());
    emit entityChaseStarted(entity, distBW);
}

/// Executed whenever an entity leaves the fov of the controlled entity.
/// Will unset the leaving entity as the target entity.
void ECChaser::onEntityLeavesFOV_(Entity *entity)
{
    qDebug() << "onEntityLeavesFOV_ executed"; // TODO: remove, test

    // if leaving entity isn't the target of controlled entity, do nothing
    if (entity != targetEntity_)
        return;

    // other wise

    // unset as target
    disconnectFromTargetSignals_();
    targetEntity_ = nullptr;

    chaseTimer_->stop(); // stop moving

    // if there is another enemy in view, target that one
    std::unordered_set<Entity*> otherEntitiesInView = fovEmitter_->entitiesInView();
    for (Entity* possibleEnemy:otherEntitiesInView){
        if (chasees_.find(possibleEnemy) != std::end(chasees_)){
            onEntityEntersFOV_(possibleEnemy);
            return;
        }
    }
}

/// Executed whenever the controlled entity moves towards its chase target.
void ECChaser::onEntityMoved_()
{
    // do nothing if nothing being chased
    if (targetEntity_.isNull())
        return;

    double distBW = distance(entityControlled()->pos(),targetEntity_->pos());
    emit entityChaseContinued(targetEntity_,distBW);
}

/// Executed whenever the controlled entity has just reached the stop distance from the chased entity.
/// Will stop moving towards the chased entity.
void ECChaser::onEntityEntersRange_(Entity *watched, Entity *watching, double range)
{
    qDebug() << "onEntityEntersRange_ executed"; // TODO: remove test
    pathMover_->stopMovingEntity();
    paused_ = true;
}

/// Executed whenever the chased entity just leaves stop distance from controlled entity.
/// Will start chasing it again.
void ECChaser::onEntityLeavesRange_(Entity *watched, Entity *watching, double range)
{
    qDebug() << "onEntityLeavesRange_ executed"; // TODO: remove test
    paused_ = false;
}

/// Executed when the chasing entity dies.
/// Will stop chasing.
void ECChaser::onChasingEntityDies_(QObject *entity)
{
    stopChasing();
}

/// Executed when the chased entity dies.
/// Will stop chasing.
void ECChaser::onChasedEntityDies_(QObject *entity)
{
    stopChasing();
}

/// Executed when the controlled entity (chasing entity) leaves a map.
/// If it left to no map (i.e. map ptr is now nullptr), will stop chasing.
void ECChaser::onChasingEntityLeavesMap_(Entity *entity)
{
    if (entity->map() == nullptr)
        stopChasing();
}

/// Executed when the chased entity leaves a Map.
/// If it left to no map (i.e. map ptr is now nullptr), will stop chasing.
void ECChaser::onChasedEntityLeavesMap_(Entity *entity)
{
    if (entity->map() == nullptr)
        stopChasing();
}

/// Executed when a chasee is destroyed.
/// Will remove it from our list of chasees.
void ECChaser::onChaseeDestroyed_(QObject *chasee)
{
    chasees_.erase((Entity*)chasee);
}

/// Takes controlled entity one step closer to chase victim :P (if chasing something).
void ECChaser::chaseStep_()
{
    // if whats being chased has died, stop chasing
    // if the thing chasing has died, stop chasing
    if (targetEntity_.isNull() || entityControlled() == nullptr){
        stopChasing();
        return;
    }

    // make sure entity and one being chased are in a map
    Map* entitysMap = entityControlled()->map();
    Map* chaseVictimsMap = targetEntity_->map();
    assert(entitysMap != nullptr && chaseVictimsMap != nullptr);

    // order to move towards chase victim :P
    if (!paused_)
        pathMover_->moveEntity(targetEntity_->pos());
}

void ECChaser::connectToTargetSignals_()
{
    // listen to when target entity gets destroyed (so we can stop chasing)
    disconnect(0,&QObject::destroyed,this,&ECChaser::onChasedEntityDies_);
    connect(targetEntity_,&QObject::destroyed,this,&ECChaser::onChasedEntityDies_);

    // listen to when target entity leaves map (so we can stop chasing)
    disconnect(0,&Entity::mapLeft,this,&ECChaser::onChasedEntityLeavesMap_);
    connect(targetEntity_,&Entity::mapLeft,this,&ECChaser::onChasedEntityLeavesMap_);

    // listen to when the target entity enters/leaves stop distance (so we can stop moving)
    entityControlled()->map()->game()->addWatchedEntity(targetEntity_,entityControlled(),stopDistance_);
}

void ECChaser::disconnectFromTargetSignals_()
{
    disconnect(0,&QObject::destroyed,this,&ECChaser::onChasedEntityDies_);
    disconnect(0,&Entity::mapLeft,this,&ECChaser::onChasedEntityLeavesMap_);
    entityControlled()->map()->game()->removeWatchedEntity(targetEntity_,entityControlled());
}
