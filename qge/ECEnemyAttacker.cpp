#include "ECEnemyAttacker.h"

#include "ECChaser.h"
#include "Slot.h"
#include "Weapon.h"
#include "WeaponSlot.h"
#include "Utilities.h"

using namespace qge;

ECAttacker::ECAttacker(Entity *entity):
    EntityController(entity),
    controllerChaseEnemies_(new ECChaser(entity))
{
    // listen to chaser
    connect(controllerChaseEnemies_, &ECChaser::entityChaseContinued,this,&ECAttacker::onEnemyChaseContinued);
    connect(controllerChaseEnemies_,&ECChaser::entityChaseStarted,this,&ECAttacker::onEnemyChaseContinued);
}

void ECAttacker::addAttackee(Entity *entity)
{
    controllerChaseEnemies_->addChasee(entity);
}

void ECAttacker::removeAttackee(Entity *entity)
{
    controllerChaseEnemies_->removeChasee(entity);
}

std::unordered_set<Entity *> ECAttacker::attackies() const
{
    return controllerChaseEnemies_->chasees();
}

/// Executed whenever the controlled entity takes one step closer to chased entity.
/// Will see if close enough to use weapon. If so, will use it.
void ECAttacker::onEnemyChaseContinued(Entity *entityChased, double distance)
{
    // TODO only attack if close to range of weapon
    // pick first weapon in controlled enities slots and use it to attack chased entity
    for (Slot* slot: entityControlled()->getSlots()){
        WeaponSlot* asWS = dynamic_cast<WeaponSlot*>(slot);
        if (asWS){
            if (asWS->isFilled()){
                Weapon* weapon = dynamic_cast<Weapon*>(asWS->item());
                if (weapon){
                    if (distance < weapon->castRange()){
                        weapon->attack(entityChased->pos());
                        emit attacked(entityChased);
                    }
                }
            }
        }
    }
}
