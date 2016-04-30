#include "ProjectileCollisionBehaviorDamage.h"
#include "Projectile.h"
#include "Map.h"

ProjectileCollisionBehaviorDamage::ProjectileCollisionBehaviorDamage():
    damage_(5)
{

}

void ProjectileCollisionBehaviorDamage::onCollisionWith(std::unordered_set<Entity *> entities)
{
    // executed every time the projectile collides with some entities
    // - traverse through, if not on nodamage list, kill it
    for (Entity* entity:entities){
        if (!projectile_->isInNoDamageList(entity)){
            projectile_->damage(entity,damage_);
        }
    }
}
