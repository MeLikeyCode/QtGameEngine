#include "ProjectileCollisionBehaviorDamage.h"
#include "Projectile.h"
#include "Map.h"

void ProjectileCollisionBehaviorDamage::onCollisionWith(std::unordered_set<Entity *> entities)
{
    // executed every time the projectile collides with some entities
    // - traverse through, if not on nodamage list, kill it
    for (Entity* entity:entities){
        if (!projectile_->isInNoDamageList(entity)){
            projectile_->map()->removeEntity(entity);
            delete entity;
        }
    }
}
