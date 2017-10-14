#include "FireballOrb.h"

#include "TopDownSprite.h"
#include "SpearProjectile.h"
#include "Sound.h"
#include "Inventory.h"
#include "cassert"
#include "Map.h"
#include "CBDamage.h"

FireballOrb::FireballOrb()
{
    // default sprite
    TopDownSprite* spr = new TopDownSprite(QPixmap(":resources/graphics/effects/fireball.png"));
    setSprite(spr);

    // default projectile spawn point (center of sprite)
    resetProjectileSpawnPoint();

    // default attachment point (center of sprite)
    QPointF pt;
    pt.setX(spr->currentlyDisplayedFrame().width()/2);
    pt.setY(spr->currentlyDisplayedFrame().height()/2);
    setAttachmentPoint(pt);

    // default sound (fireball.wav)
    soundEffect_ = new Sound("qrc:/resources/sounds/fireball.wav",this);

    // default range
    setCastRange(400);
}

/// Spawns a projectile towards the specified position.
/// The specified position is in map coordinates.
void FireballOrb::attack(QPointF position)
{
    soundEffect_->play(1);

    Entity* owningEntity = inventory()->entity();   // Entity that owns this weapon
    assert(owningEntity != nullptr);

    Map* map = owningEntity->map();                 // Map that the owner is in
    assert(map != nullptr);

    // create a spear projectile
    QPointF startPos = mapToMap(projectileSpawnPoint());

    SpearProjectile* spearProjectile = new SpearProjectile(600,50);
    TopDownSprite* spr = new TopDownSprite(QPixmap(":resources/graphics/effects/fireball.png"));
    spearProjectile->setSprite(spr);
    map->addEntity(spearProjectile);
    CBDamage* collisionBehavior = (CBDamage*)spearProjectile->collisionBehavior();
    collisionBehavior->addCollisionToIgnore(owningEntity,spearProjectile);
    collisionBehavior->addCollisionToIgnore(spearProjectile,this);
    spearProjectile->setPos(startPos);
    spearProjectile->shootTowards(position);
    spearProjectile->setZ(owningEntity->z() + owningEntity->height());
}
