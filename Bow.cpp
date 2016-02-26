#include "Bow.h"
#include "StraightProjectile.h"
#include "Inventory.h"
#include "Map.h"

Bow::Bow()
{
    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/bow.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // default projectile spawn point
    resetProjectileSpawnPoint();

    // default attachment point
    QPointF pt;
    pt.setX(pm_.width()/2);
    pt.setY(pm_.height()/2);
    setAttachmentPoint(pt);
}

/// Spawns a projectile towards the targetPoint.
/// taretPoint is in map coordinates.
void Bow::attack(QPointF targetPoint)
{
    // entity that owns this bow
    Entity* owningEntity = inventory()->entity();

    // create projectile
    StraightProjectile* projectile = new StraightProjectile();
    map()->addEntity(projectile);

    // place projectile at projectile spawn point of bow
    projectile->setPointPos(mapToMap(projectileSpawnPoint()));
    double rx = 0;
    double ry = projectile->sprite()->boundingRect().height()/2;
    projectile->setRotationPoint(QPointF(rx,ry));
    projectile->setFacingAngle(owningEntity->facingAngle());

    // make sure owning entity is not killed by this projectile
    projectile->addToNoDamageList(owningEntity);

    // set where it should go to
    projectile->setTargetPoint(targetPoint);

    // make it go
    projectile->start();
}
