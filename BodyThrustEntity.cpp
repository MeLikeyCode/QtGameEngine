#include "BodyThrustEntity.h"
#include <QTimer>
#include <cassert>
#include "BodyThrust.h"
#include "Sprite.h"

BodyThrustEntity::BodyThrustEntity()
{
    // connect timer to check the field of view every once in a while
    timerCheckFov_ = new QTimer(this);
    connect(timerCheckFov_,&QTimer::timeout,this,&BodyThrustEntity::checkFov_);
    timerCheckFov_->start(CHECK_FOV_FREQUENCY_MS);

    // connect a timer to attack every once in a while (if in range of an enemy)
    attackTimer_ = new QTimer(this);
    connect(attackTimer_,&QTimer::timeout,this,&BodyThrustEntity::attackIfInRange_);
    attackTimer_->start(ATTACK_FREQUENCY_MS);

    targetEntity_ = nullptr;

    // sprite
    Sprite* spr = new Sprite();
    setSprite(spr);
    spr->addFrames(":resources/graphics/spider",1,"stand"); // stand anim
    spr->addFrames(":resources/graphics/spider",7,"walk");  // walk anim
    spr->play("stand",1,1); // play stand anim

    // BodyThrust
    bodyThrust_ = new BodyThrust(this);
}

BodyThrustEntity::~BodyThrustEntity()
{
    timerCheckFov_->disconnect();
    attackTimer_->disconnect();
}

/// Adds an enemy group to the BodyThrustEntity.
void BodyThrustEntity::addEnemy(int groupID)
{
    enemyGroups_.insert(groupID);
}

/// Returns true if the specified group is an enemy to this BodyThrustEntity.
bool BodyThrustEntity::isEnemy(int groupID)
{
    return enemyGroups_.count(groupID) > 0;
}

/// Returns true if the specified Entity is an enemy.
bool BodyThrustEntity::isEnemy(Entity *entity)
{
    return enemyGroups_.count(entity->groupID()) > 0;
}

/// Removes the specified group from the enemies list of this BodyThrustEntity.
void BodyThrustEntity::removeEnemy(int groupID)
{
    enemyGroups_.erase(groupID);
}

/// Executed periodically.
/// Checks the fov, if there is an enemy, will move to its location while attacking.
void BodyThrustEntity::checkFov_()
{
    if (isMoving()){
        return; // prevent retargetting frequently (allows focusing on one enemy)
    }

    for (Entity* entity:entitiesInView()){
        if (isEnemy(entity) && entity != this){
            targetEntity_ = entity;

            // get notified when target dies
            connect(targetEntity_,&QObject::destroyed,this,&BodyThrustEntity::onTargetDelete_);

            this->moveTo(entity->pointPos());
            timerCheckFov_->disconnect();

            QTimer::singleShot(2000,this,SLOT(startCheckingFov_()));
        }
    }
}

/// Starts firing the checkFov_ function periodically.
void BodyThrustEntity::startCheckingFov_()
{
    connect(timerCheckFov_,&QTimer::timeout,this,&BodyThrustEntity::checkFov_);
    timerCheckFov_->start(500);
}

/// If the Enemy is in range of an enemy will attack with BodyThrust.
void BodyThrustEntity::attackIfInRange_()
{
    // don't do anything if doesn't have a target
    if (targetEntity_ == nullptr){
        return;
    }

    double bodyThrustRange = bodyThrust_->thrustDistance();
    double distanceToE = QLineF(pointPos(),targetEntity_->pointPos()).length();

    if (distanceToE < bodyThrustRange*4){
        rotateTo(targetEntity_->pointPos());
        bodyThrust_->use();
    }
}

/// Executed when the target of this BodyThrustEntity gets deleted.
/// Stops referencing it (prevents dangling pointer related memory curruption)
void BodyThrustEntity::onTargetDelete_()
{
    targetEntity_ = nullptr;
}
