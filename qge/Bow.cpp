#include "Bow.h"
#include "Projectile.h"
#include "Inventory.h"
#include "Map.h"
#include "SpearProjectile.h"
#include "Sprite.h"
#include "Entity.h"
#include "Sound.h"
#include "CBDamage.h"
#include "TopDownSprite.h"

using namespace qge;

Bow::Bow()
{
    // default sprite
    TopDownSprite* spr = new TopDownSprite(QPixmap(":resources/graphics/weapons/bow.png"));
    setSprite(spr);

    // default projectile spawn point (center of sprite)
    resetProjectileSpawnPoint();

    // default attachment point
    QPointF pt;
    pt.setX(spr->currentlyDisplayedFrame().width()/2);
    pt.setY(spr->currentlyDisplayedFrame().height()/2);
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

    Entity* owningEntity = inventory()->entity();   // Entity that owns this bow
    assert(owningEntity != nullptr);

    Map* map = owningEntity->map();                 // Map that the owner is in
    assert(map != nullptr);

    // create a spear projectile
    QPointF startPos = mapToMap(projectileSpawnPoint());

    SpearProjectile* spearProjectile = new SpearProjectile(600,50);
    spearProjectile->addEntityToNotCollideWith(this); // do not collide with bow
    spearProjectile->addEntityToNotCollideWith(owningEntity); // do not collide with owner

    map->addEntity(spearProjectile);

    spearProjectile->setPos(startPos);
    spearProjectile->shootTowards(targetPoint);
    spearProjectile->setZ(owningEntity->z() + owningEntity->height());
}
