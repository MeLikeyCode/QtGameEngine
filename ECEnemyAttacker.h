#ifndef ECATTACKENEMIESWITHWEAPON_H
#define ECATTACKENEMIESWITHWEAPON_H

#include <QPointer>
#include <QObject>
#include <memory>

#include "Entity.h"
#include "EntityController.h"

class ECEnemyChaser;

/// An EntityController that makes the controlled entity attack enemies
/// within its field of view.
///
/// Currently the attack policy is: The controlled Entity will use
/// the Weapon in its first WeaponSlot. If the controlled entity does not have
/// any WeaponSlots with a Weapon, it will simply not attack. But later, I can
/// make the attack policy settable somehow (maybe via strategy pattern).
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECAttackEnemiesWithWeapon* c = new ECAttackEnemiesWithWeapon(entity);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// That is all. From then on, the controlled entity will use its first viable
/// weapon to attack any enemies in its field of view.
/// @see EntityController
class ECEnemyAttacker: public EntityController
{
    Q_OBJECT
public:
    ECEnemyAttacker(Entity* entity);
public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);
signals:
    /// Emitted each time the controlled entity attacks an enemy.
    /// Note that this is only emitted when the controlled entity actually
    /// tries to *attack* an entity (and not simply chase it).
    void attacked(Entity* entityAttacked);
private:
    std::unique_ptr<ECEnemyChaser> controllerChaseEnemies_;
};

#endif // ECATTACKENEMIESWITHWEAPON_H
