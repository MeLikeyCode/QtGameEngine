#include "Bow.h"
#include "Projectile.h"
#include "Inventory.h"
#include "Map.h"
#include "SpearProjectile.h"
#include "Sprite.h"
#include "Entity.h"
#include "Sound.h"

Bow::Bow()
{
    // default sprite
    Sprite* spr = new Sprite(QPixmap(":resources/graphics/weapons/bow.png"));
    setSprite(spr);

    // default projectile spawn point (center of sprite)
    resetProjectileSpawnPoint();

    // default attachment point
    QPointF pt;
    pt.setX(spr->currentFrame().width()/2);
    pt.setY(spr->currentFrame().height()/2);
    setAttachmentPoint(pt);

    // default sound effect
    soundEffect_ = new Sound("qrc:/resources/sounds/bow.wav",this);

    // range
    setCastRange(400);
}

/// Spawns a projectile towards the targetPoint.
/// taretPoint is in map coordinates.
void Bow::attack(QPointF targetPoint)
{   
    soundEffect_->play(1);

    Entity* owningEntity = inventory()->entity();     // Entity that owns this bow
    Map* map = owningEntity->map();                          // Map that the owner is in

    // create a spear projectile
    QPointF startPos = mapToMap(projectileSpawnPoint());

    std::unordered_set<Entity*> noDamageList;
    noDamageList.insert(owningEntity);
    noDamageList.insert(this);

    SpearProjectile* spearProjectile = new SpearProjectile(600,5);
    map->addEntity(spearProjectile);
    spearProjectile->setPointPos(startPos);
    spearProjectile->shootTowards(targetPoint);
    spearProjectile->setPointZ(owningEntity->pointZ() + owningEntity->height());

//    // create projectile (using Projectile and instantiating all the behaviors, etc...)
//    ProjectileMoveBehaviorSine* mb = new ProjectileMoveBehaviorSine(100,200,1000,targetPoint);
//    ProjectileCollisionBehaviorDamage* cb = new ProjectileCollisionBehaviorDamage(5);

//    QPointF start = mapToMap(projectileSpawnPoint());

//    Sprite* sprite = new Sprite();
//    QPixmap pm = QPixmap(":resources/graphics/weapons/spear.png");
//    sprite->setPixmap(pm);

//    std::unordered_set<Entity*> noDamageList;
//    noDamageList.insert(owningEntity);
//    noDamageList.insert(this);

//    Projectile* projectile = new Projectile(start,mb,cb,sprite,noDamageList,map);

//    projectile->setStepSize(10);
//    projectile->setStepFrequency(13);

//    projectile->startMoving();
}
