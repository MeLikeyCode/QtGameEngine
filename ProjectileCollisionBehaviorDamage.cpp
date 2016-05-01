#include "ProjectileCollisionBehaviorDamage.h"
#include "Projectile.h"
#include "Map.h"
#include <QDebug>

ProjectileCollisionBehaviorDamage::ProjectileCollisionBehaviorDamage(int damageAmount):
    damage_(damageAmount)
{

}

void ProjectileCollisionBehaviorDamage::onCollisionWith(std::unordered_set<QPointer<Entity>> entities)
{
    // executed every time the projectile collides with some entities
    // - traverse through, if not on nodamage list, kill it
    for (QPointer<Entity> entity:entities){
        if (entity.isNull() == false){
            if (!projectile_->isInNoDamageList(entity)){
                projectile_->damage(entity,damage_);
            }
        }
    }
}
