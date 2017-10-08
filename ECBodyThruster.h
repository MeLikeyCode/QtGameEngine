#ifndef ECBODYTHRUSTER_H
#define ECBODYTHRUSTER_H

#include <QPointer>
#include <QObject>
#include "Entity.h"
#include <memory>

class ECChaseEnemies;
class BodyThrust;

/// An entity controller that causes the controlled entity to chase enemy entites
/// in its field of view and BodyThrust them when close enough.
///
/// Example usage:
/// ECBodyThruster* c = new ECBodyThruster(controlledEntity);
class ECBodyThruster: public QObject
{
    Q_OBJECT
public:
    ECBodyThruster(Entity &entity);

public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);

signals:
    /// Emitted each time the controlled entity thrusts towards an enemy.
    void thrusted(Entity* towardsEnemy);

private:
    QPointer<Entity> entity_;
    std::unique_ptr<ECChaseEnemies> controllerChaseEnemies_;
    std::unique_ptr<BodyThrust> bodyThrustAbility_;
};

#endif // ECBODYTHRUSTER_H
