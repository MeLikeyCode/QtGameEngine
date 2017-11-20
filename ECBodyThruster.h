#ifndef ECBODYTHRUSTER_H
#define ECBODYTHRUSTER_H

#include <QPointer>
#include <QObject>
#include <memory>

#include "EntityController.h"
#include "Entity.h"

class ECEnemyChaser;
class BodyThrust;

/// An entity controller that causes the controlled entity to chase enemy entites
/// in its field of view and use the BodyThrust ability on them when close enough.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECBodyThruster* c = new ECBodyThruster(controlledEntity);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ECBodyThruster: public EntityController
{
    Q_OBJECT
public:
    ECBodyThruster(Entity* entity);

public slots:
    void onEnemyChaseContinued(Entity* entityChased, double distance);

signals:
    /// Emitted each time the controlled entity thrusts towards an enemy.
    void thrusted(Entity* towardsEnemy);

private:
    ECEnemyChaser* controllerChaseEnemies_;
    BodyThrust* bodyThrustAbility_;
};

#endif // ECBODYTHRUSTER_H
