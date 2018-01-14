#include "FireballLauncher.h"

#include "TopDownSprite.h"
#include "SpearProjectile.h"
#include "Sound.h"
#include "Inventory.h"
#include "cassert"
#include "Map.h"
#include "CBDamage.h"

FireballLauncher::FireballLauncher()
{
    // default sprite
    TopDownSprite* spr = new TopDownSprite(QPixmap(":resources/graphics/effects/fireball.png"));
    setSprite(spr);
    setDescription("A Weapon that when used will launch fireballs at the clicked location.");

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
void FireballLauncher::attack(QPointF position)
{
    soundEffect_->play(1);

    Entity* owningEntity = inventory()->entity();   // Entity that owns this weapon
    assert(owningEntity != nullptr);

    Map* map = owningEntity->map();                 // Map that the owner is in
    assert(map != nullptr);

    // create a spear projectile
    QPointF startPos = mapToMap(projectileSpawnPoint());

    SpearProjectile* fireballProjectile = new SpearProjectile(600,50);
    TopDownSprite* spr = new TopDownSprite(QPixmap(":resources/graphics/effects/fireball.png"));
    fireballProjectile->setSprite(spr);
    map->addEntity(fireballProjectile);
    CBDamage* collisionBehavior = (CBDamage*)fireballProjectile->collisionBehavior();
    collisionBehavior->addCollisionToIgnore(owningEntity,fireballProjectile);
    collisionBehavior->addCollisionToIgnore(fireballProjectile,this);
    fireballProjectile->setPos(startPos);
    fireballProjectile->shootTowards(position);
    fireballProjectile->setZ(owningEntity->z() + owningEntity->height());
}
