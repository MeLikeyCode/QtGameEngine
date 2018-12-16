#include "ECChaser.h"

#include "ECFieldOfViewEmitter.h"
#include "ECPathMover.h"
#include "Map.h"
#include "Game.h"
#include "QtUtilities.h"

using namespace qge;

ECChaser::ECChaser(Entity* entity):
    EntityController(entity),
    stopDistance_(100),
    fovEmitter_(new ECFieldOfViewEmitter(entity)),
    pathMover_(new ECPathMover(entity)),
    chaseTimer_(new QTimer(this)),
    paused_(false),
    targetEntity_(nullptr)
{
    Map* entitysMap = entity->map();
    if (entitysMap){
        Game* entitysGame = entitysMap->game();
        if (entitysGame){
            // listen to game
            connect(entitysGame, &Game::watchedEntityEntersRange, this, &ECChaser::onStopRangeEntered_);
            connect(entitysGame, &Game::watchedEntityLeavesRange, this, &ECChaser::onStopRangeLeft_);
        }
    }

    // listen to when the controlled entity enters a map
    connect(entity,&Entity::mapEntered,this,&ECChaser::onControlledEntityEntersMap_);

    // listen to fov emitter
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityEntersFOV,this,&ECChaser::onEntityEntersFOV_);
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityLeavesFOV,this,&ECChaser::onEntityLeavesFOV_);

    // listen to path mover
    connect(pathMover_,&ECPathMover::moved,this,&ECChaser::onControlledEntityMoved_);

    // listen to when the controlled entity dies
    connect(entityControlled(),&QObject::destroyed,this,&ECChaser::onControlledEntityDies_);

    // listen to when controlled entity leaves map
    connect(entityControlled(),&Entity::mapLeft,this, &ECChaser::onControlledEntityLeavesMap_);

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

/// Determines whether the field of view of the controlled enity is shown or not.
void ECChaser::setShowFOV(bool tf)
{
    fovEmitter_->setShowFOV(tf);
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
    if (chasees_.find(entity) == std::end(chasees_) && Game::game->diplomacyManager().getRelationship(entityControlled()->group(),entity->group()) != Relationship::ENEMY)
        return;

    // set entering entity as target entity and connect to some signals from it
    targetEntity_ = entity;
    connectToTargetSignals_();

    chaseStep_();
    chaseTimer_->start(2000); // TODO: store in a (modifiable) variable somewhere

    double distBW = QtUtils::distance(entityControlled()->pos(),entity->pos());
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
void ECChaser::onControlledEntityMoved_()
{
    // do nothing if nothing being chased
    if (targetEntity_.isNull())
        return;

    if (!paused_){
        double distBW = QtUtils::distance(entityControlled()->pos(),targetEntity_->pos());
        emit entityChaseContinued(targetEntity_,distBW);
    }
}

/// Executed whenever the controlled entity has just reached the stop distance from the chased entity.
/// Will stop moving towards the chased entity.
void ECChaser::onStopRangeEntered_(Entity *watched, Entity *watching, double range)
{
    qDebug() << "onEntityEntersRange_ executed"; // TODO: remove test
    pathMover_->stopMovingEntity();
    paused_ = true;
    emit entityChasePaused(targetEntity_);
}

/// Executed whenever the chased entity just leaves stop distance from controlled entity.
/// Will start chasing it again.
void ECChaser::onStopRangeLeft_(Entity *watched, Entity *watching, double range)
{
    qDebug() << "onEntityLeavesRange_ executed"; // TODO: remove test
    paused_ = false;
}

/// Executed when the chasing entity dies.
/// Will stop chasing.
void ECChaser::onControlledEntityDies_(QObject *entity)
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
void ECChaser::onControlledEntityLeavesMap_(Entity *entity)
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

/// Executed when the controlled entity enters a map.
/// If the entered map has a game, we will listen to events from the game.
void ECChaser::onControlledEntityEntersMap_()
{
    Game* mapsGame = entityControlled()->map()->game();
    if (mapsGame){
        disconnect(0, &Game::watchedEntityEntersRange, this, &ECChaser::onStopRangeEntered_);
        disconnect(0, &Game::watchedEntityLeavesRange, this, &ECChaser::onStopRangeLeft_);
        connect(mapsGame, &Game::watchedEntityEntersRange, this, &ECChaser::onStopRangeEntered_);
        connect(mapsGame, &Game::watchedEntityLeavesRange, this, &ECChaser::onStopRangeLeft_);
    }
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
