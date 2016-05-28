#include "AIEntity.h"
#include <QTimer>
#include <QDebug> // TODO: remove
#include <cassert>
#include "WeaponSlot.h"
#include "Bow.h"
#include "Spear.h"
#include "Axe.h"

AIEntity::AIEntity()
{
    timerCheckFov_ = new QTimer(this);
    connect(timerCheckFov_,&QTimer::timeout,this,&AIEntity::checkFov_);
    timerCheckFov_->start(500);

    attackTimer_ = new QTimer(this);
    connect(attackTimer_,&QTimer::timeout,this,&AIEntity::swingIfInRange_);
    attackTimer_->start(1500);

    targetEntity_ = nullptr;

    // sprite
    Sprite* spr = new Sprite();
    setSprite(spr);
    spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
    spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
    spr->play("stand",1,1); // play stand anim

    // some attachment points
    addNamedPoint(QPointF(24,5),"left shoulder");
    addNamedPoint(QPointF(24,58),"right shoulder");
    addNamedPoint(QPointF(50,30),"center");

    // some slots
    // give it some Weapons
    WeaponSlot* lhs = new WeaponSlot();
    lhs->setName("lhs");
    lhs->setPosition(namedPoint("left shoulder"));
    WeaponSlot* rhs = new WeaponSlot();
    lhs->setName("rhs");
    lhs->setPosition(namedPoint("right shoulder"));
    WeaponSlot* cs = new WeaponSlot();
    lhs->setName("cs");
    lhs->setPosition(namedPoint("center"));

    addSlot(lhs);
    addSlot(rhs);
    addSlot(cs);

    // a weapon
//    Bow* bow = new Bow();
//    addItemToInventory(bow);
//    equipItem(bow,cs);

//    Spear* spear = new Spear();
//    addItemToInventory(spear);
//    equipItem(spear,rhs);

    Axe* axe = new Axe();
    addItemToInventory(axe);
    equipItem(axe,lhs);

    // default weapon
    setDefaultWeapon(axe);
}

AIEntity::~AIEntity()
{
    timerCheckFov_->disconnect();
    attackTimer_->disconnect();
}

/// Returns the default weapon equipped by the entity. If the Entity does not
/// have a weapon equipped, returns nullptr.
Weapon *AIEntity::defaultWeapon()
{
    return this->defaultWeapon_;
}

void AIEntity::setDefaultWeapon(Weapon *weapon)
{
    // make sure the weapon is in the inventory of the Enemy
    // assert(inventoryContains(weapon)); TODO: find out why this line gives error

    defaultWeapon_ = weapon;


}

/// Adds an enemy group to the AIEntity.
void AIEntity::addEnemy(int groupID)
{
    enemyGroups_.insert(groupID);
}

/// Returns true if the specified group is an enemy to this AIEntity.
bool AIEntity::isEnemy(int groupID)
{
    return enemyGroups_.count(groupID) > 0;
}

/// Returns true if the specified Entity is an enemy.
bool AIEntity::isEnemy(Entity *entity)
{
    return enemyGroups_.count(entity->groupID()) > 0;
}

/// Removes the specified group from the enemies list of this AIEntity.
void AIEntity::removeEnemy(int groupID)
{
    enemyGroups_.erase(groupID);
}

/// Checks the fov, if there is an enemy, will move to its location while
/// swinging.
void AIEntity::checkFov_()
{
    if (isMoving()){
        return;
    }

    for (Entity* entity:entitiesInView()){
        if (isEnemy(entity) && entity != this){
            targetEntity_ = entity;

            // get notified when target dies
            connect(targetEntity_,&QObject::destroyed,this,&AIEntity::onTargetDelete_);

            this->moveTo(entity->pointPos());
            timerCheckFov_->disconnect();

            QTimer::singleShot(2000,this,SLOT(startCheckingFov_()));
        }
    }
}

/// Starts firing the checkFov_ function periodically.
void AIEntity::startCheckingFov_()
{
    connect(timerCheckFov_,&QTimer::timeout,this,&AIEntity::checkFov_);
    timerCheckFov_->start(500);
}

/// If the Enemy is in range of an enemy will attack with its def weapon.
void AIEntity::swingIfInRange_()
{
    // don't do anything if doesn't have a target
    if (targetEntity_ == nullptr){
        return;
    }

    double weaponRange = defaultWeapon()->castRange();
    double distanceToE = QLineF(pointPos(),targetEntity_->pointPos()).length();

    if (distanceToE < weaponRange*4){
        rotateTo(targetEntity_->pointPos());
        defaultWeapon()->attack(targetEntity_->pointPos());
    }
}

/// Executed when the target of this Enemy gets deleted.
/// Stops referencing it (prevents dangling pointer related memory curruption)
void AIEntity::onTargetDelete_()
{
    targetEntity_ = nullptr;
}
