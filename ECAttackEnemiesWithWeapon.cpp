#include "ECAttackEnemiesWithWeapon.h"

#include "ECChaseEnemies.h"
#include <cassert>
#include "Slot.h"
#include "Weapon.h"
#include "WeaponSlot.h"

ECAttackEnemiesWithWeapon::ECAttackEnemiesWithWeapon(Entity *entity):
    entity_(entity),
    controllerChaseEnemies_(new ECChaseEnemies(entity))
{
    assert(entity != nullptr);

    // listen to chaser
    connect(controllerChaseEnemies_,&ECChaseEnemies::entityChaseContinued,this,&ECAttackEnemiesWithWeapon::onEnemyChaseContinued);
    connect(controllerChaseEnemies_,&ECChaseEnemies::entityChaseStarted,this,&ECAttackEnemiesWithWeapon::onEnemyChaseContinued);
}

/// Executed whenever the controlled entity takes one step closer to chased entity.
/// Will see if close enough to use weapon. If so, will use it.
void ECAttackEnemiesWithWeapon::onEnemyChaseContinued(Entity *entityChased, double distance)
{
    // TODO only attack if close to range of weapon
    // pick first weapon in controlled enities slots and use it to attack chased entity
    for (Slot* slot: entity_->getSlots()){
        WeaponSlot* asWS = dynamic_cast<WeaponSlot*>(slot);
        if (asWS){
            if (asWS->isFilled()){
                Weapon* weapon = dynamic_cast<Weapon*>(asWS->item());
                if (weapon){
                    weapon->attack(entityChased->pointPos());
                }
            }
        }
    }
}
