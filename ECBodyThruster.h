#ifndef ECBODYTHRUSTER_H
#define ECBODYTHRUSTER_H

#include <QPointer>
#include <QObject>
#include "Entity.h"

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
    ~ECBodyThruster();

public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);

signals:
    /// Emitted each time the controlled entity thrusts towards an enemy.
    void thrusted(Entity* towardsEnemy);

private:
    QPointer<Entity> entity_;
    ECChaseEnemies* controllerChaseEnemies_;
    BodyThrust* bodyThrustAbility_;
};

#endif // ECBODYTHRUSTER_H
