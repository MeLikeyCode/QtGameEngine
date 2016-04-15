#include "Enemy.h"
#include <QTimer>
#include <QDebug> // TODO: remove
#include <cassert>
#include "WeaponSlot.h"
#include "Bow.h"
#include "Spear.h"
#include "Axe.h"

Enemy::Enemy()
{
    timerCheckFov_ = new QTimer();
    connect(timerCheckFov_,&QTimer::timeout,this,&Enemy::checkFov_);
    timerCheckFov_->start(500);

    attackTimer_ = new QTimer();
    connect(attackTimer_,&QTimer::timeout,this,&Enemy::swingIfInRange_);
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
    rhs->setName("rhs");
    rhs->setPosition(namedPoint("right shoulder"));

    WeaponSlot* cs = new WeaponSlot();
    cs->setName("cs");
    cs->setPosition(namedPoint("center"));

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

Enemy::~Enemy()
{
    timerCheckFov_->disconnect();
    attackTimer_->disconnect();
}

/// Returns the default weapon equipped by the entity. If the Entity does not
/// have a weapon equipped, returns nullptr.
Weapon *Enemy::defaultWeapon()
{
    return this->defaultWeapon_;
}

void Enemy::setDefaultWeapon(Weapon *weapon)
{
    // make sure the weapon is in the inventory of the Enemy
    // assert(inventoryContains(weapon)); TODO: find out why this line gives error

    defaultWeapon_ = weapon;


}

/// Checks the fov, if there is an enemy, will move to its location.
void Enemy::checkFov_()
{
    for (Entity* entity:entitiesInView()){
        bool isFriend = false; // TODO: determine if entity is friend
        if (!isFriend && entity != this){
            targetEntity_ = entity;

            // get notified when target dies
            connect(targetEntity_,&QObject::destroyed,this,&Enemy::onTargetDelete_);

            this->moveTo(entity->pointPos());
            timerCheckFov_->disconnect();

            QTimer::singleShot(2000,this,SLOT(startCheckingFov_()));
        }
    }
}

/// Starts firing the checkFov_ function periodically.
void Enemy::startCheckingFov_()
{
    connect(timerCheckFov_,&QTimer::timeout,this,&Enemy::checkFov_);
    timerCheckFov_->start(500);
}

/// If the Enemy is in range of an enemy will attack with its def weapon.
void Enemy::swingIfInRange_()
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
void Enemy::onTargetDelete_()
{
    targetEntity_ = nullptr;
}
