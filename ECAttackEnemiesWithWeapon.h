#ifndef ECATTACKENEMIESWITHWEAPON_H
#define ECATTACKENEMIESWITHWEAPON_H

#include <QPointer>
#include <QObject>
#include "Entity.h"
#include <memory>

class ECChaseEnemies;

/// An entity controller that makes the controlled entity attack enemies
/// within its field of view with its weapon. The controlled Entity will use
/// the Weapon in its first WeaponSlot. If the controlled entity does not have
/// any WeaponSlots with a Weapon, it will simply not attack.
///
/// Example usage:
/// ECAttackEnemiesWithWeapon* c = new ECAttackEnemiesWithWeapon(entity);
///
/// That is all. From then on, the controlled entity will use its first viable
/// weapon to attack any enemies in its field of view.
class ECAttackEnemiesWithWeapon: public QObject
{
    Q_OBJECT
public:
    ECAttackEnemiesWithWeapon(Entity &entity);
public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);
signals:
    /// Emitted each time the controlled entity attacks an enemy.
    /// Note that this is only emitted when the controlled entity actually
    /// tries to *attack* an entity (and not simply chase it).
    void attacked(Entity* entityAttacked);
private:
    QPointer<Entity> entity_;
    std::unique_ptr<ECChaseEnemies> controllerChaseEnemies_;
};

#endif // ECATTACKENEMIESWITHWEAPON_H
