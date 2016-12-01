#ifndef ECATTACKENEMIESWITHWEAPON_H
#define ECATTACKENEMIESWITHWEAPON_H

#include <QPointer>
#include <QObject>
#include "Entity.h"

class ECChaseEnemies;

/// An entity controller that makes the controlled entity attack enemies
/// within its field of view with its weapon.
class ECAttackEnemiesWithWeapon: public QObject
{
    Q_OBJECT
public:
    ECAttackEnemiesWithWeapon(Entity* entity);
public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);
signals:
    /// Emitted each time the controlled entity attacks an enemy.
    /// Note that this is only emitted when the controlled entity actually
    /// tries to *attack* an entity (and not simply chase it).
    void attacked(Entity* entityAttacked);
private:
    QPointer<Entity> entity_;
    ECChaseEnemies* controllerChaseEnemies_;
};

#endif // ECATTACKENEMIESWITHWEAPON_H
