#include "ECChaseEnemies.h"
#include "ECFieldOfViewEmitter.h"
#include "ECPathMover.h"
#include <cassert>
#include <QTimer>
#include "Map.h"

ECChaseEnemies::ECChaseEnemies(Entity *entity):
    entity_(entity),
    enemyGroups_(),
    fovEmitter_(new ECFieldOfViewEmitter(entity)),
    pathMover_(new ECPathMover(entity)),
    chaseTimer_(new QTimer(this)),
    shouldChase_(true),
    entityBeingChased_(nullptr)
{
    // make sure the entity is not nullptr
    assert(entity != nullptr);

    // listen to fov emitter
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityEntersFOV,this,&ECChaseEnemies::onEntityEntersFOV_);
    connect(fovEmitter_,&ECFieldOfViewEmitter::entityLeavesFOV,this,&ECChaseEnemies::onEntityLeavesFOV_);

    // listen to path mover
    connect(pathMover_,&ECPathMover::moved,this,&ECChaseEnemies::onEntityMoved_);
}

/// Add a group number that the controlled entity should consider as enemies.
/// The controlled entity will chase enemy entities.
void ECChaseEnemies::addEnemyGroup(int groupNumber)
{
    enemyGroups_.insert(groupNumber);
}

std::unordered_set<int> ECChaseEnemies::enemyGroups()
{
    return enemyGroups_;
}

/// Makes it so the controlled entity stops chasing enemy entities.
/// The controlled entity will then simply ignore any enemy entities that enter
/// its field of view.
/// If the controlled entity is currently chasing, it will stop.
void ECChaseEnemies::stopChasing()
{
    // if currently chasing stop
    if (entityBeingChased_ != nullptr){
        entityBeingChased_ = nullptr;
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

/// Returns true if the specified group number is an enemy.
bool ECChaseEnemies::isAnEnemyGroup(int groupNumber)
{
    return enemyGroups_.find(groupNumber) != enemyGroups_.end();
}

/// Executed whenever an entity enters the fov of the controlled entity.
/// Will see if that entity is an enemy and if the controlled entity should
/// start chasing it.
void ECChaseEnemies::onEntityEntersFOV_(Entity *entity)
{

    // if
    // - the controlled entity should be chasing right now
    // - the controlled entity is not currently chasing anyone
    // - the entity is an enemy
    // then
    // - chase it
    if (entityBeingChased_.isNull() && shouldChase_ && isAnEnemyGroup(entity->groupID())){
        entityBeingChased_ = entity;
        connect(chaseTimer_,&QTimer::timeout,this,&ECChaseEnemies::chaseStep_);
        chaseTimer_->start(1000); // TODO: store in a (modifiable) variable somewhere
        emit entityChaseStarted(entity,300); // TODO: replace 300 with the actual distance b/w
                                             // controlled entity and entity being chased
    }
}

/// Executed whenever an entity leaves the fov of the controlled entity.
/// If chasing it, will stop.
void ECChaseEnemies::onEntityLeavesFOV_(Entity *entity)
{
    if (entity == entityBeingChased_){
        entityBeingChased_ = nullptr;
        chaseTimer_->disconnect();
    }
}

/// Executed whenever the controlled entity moves towards its chase target.
/// Will simply emit a signal.
void ECChaseEnemies::onEntityMoved_()
{
    // if there is an entity being chased, emit updated distance
    if (!entityBeingChased_.isNull()){
        emit entityChaseContinued(entityBeingChased_,300); // TODO: replace 300 w/ actual distance
                                                           // b/w controlled entity and the one being chased
    }
}

/// Takes controlled entity one step closer to chase victim :P (if chasing something).
void ECChaseEnemies::chaseStep_()
{
    // make sure is actually chasing something
    assert(!entityBeingChased_.isNull());

    // make sure entity and one being chased are in a map
    Map* entitysMap = entity_->map();
    Map* chaseVictimsMap = entityBeingChased_->map();
    assert(entitysMap != nullptr && chaseVictimsMap != nullptr);

    // make sure is supposed to be chasing right now
    assert(shouldChase_);

    // order to move towards chase victim
    pathMover_->moveEntityTo(entityBeingChased_->pointPos());
}
