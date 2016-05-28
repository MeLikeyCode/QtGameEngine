#include "Bow.h"
#include "Projectile.h"
#include "Inventory.h"
#include "Map.h"
#include "ProjectileMoveBehaviorStraight.h" // TODO: remove, unused
#include "ProjectileCollisionBehaviorDamage.h" // TODO: remove, unused
#include "ProjectileMoveBehaviorSine.h" // TODO: remove, unused
#include "SpearProjectile.h"
#include "Sprite.h"

Bow::Bow()
{
    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/bow.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // default projectile spawn point (center of sprite)
    resetProjectileSpawnPoint();

    // default attachment point
    QPointF pt;
    pt.setX(pm_.width()/2);
    pt.setY(pm_.height()/2);
    setAttachmentPoint(pt);

    // range
    setCastRange(400);
}

/// Spawns a projectile towards the targetPoint.
/// taretPoint is in map coordinates.
void Bow::attack(QPointF targetPoint)
{

    Entity* owningEntity = inventory()->entity();     // Entity that owns this bow
    Map* map = owningEntity->map();                   // Map that the owner is in

//    // create a spear projectile
//    QPointF startPos = mapToMap(projectileSpawnPoint());

//    std::unordered_set<Entity*> noDamageList;
//    noDamageList.insert(owningEntity);
//    noDamageList.insert(this);

//    SpearProjectile* spearProjectile = new SpearProjectile(startPos, targetPoint, 300, 5,
//                                                           noDamageList, map);
//    spearProjectile->startMoving();

    // create projectile (using Projectile and instantiating all the behaviors, etc...)
    ProjectileMoveBehaviorSine* mb = new ProjectileMoveBehaviorSine(40,110,600,targetPoint);
    ProjectileCollisionBehaviorDamage* cb = new ProjectileCollisionBehaviorDamage(5);

    QPointF start = mapToMap(projectileSpawnPoint());

    Sprite* sprite = new Sprite();
    QPixmap pm = QPixmap(":resources/graphics/weapons/spear.png");
    sprite->setPixmap(pm);

    std::unordered_set<Entity*> noDamageList;
    noDamageList.insert(owningEntity);
    noDamageList.insert(this);

    Projectile* projectile = new Projectile(start,mb,cb,sprite,noDamageList,map);

    projectile->setStepSize(10);
    projectile->setStepFrequency(20);

    projectile->startMoving();
}
