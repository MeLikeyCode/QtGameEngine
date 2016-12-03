#ifndef ECCHASEENEMIES_H
#define ECCHASEENEMIES_H

#include <QObject>
#include <QPointer>
#include <unordered_set>
#include "Entity.h"

class ECPathMover;
class ECFieldOfViewEmitter;
class QTimer;

/// An entity controller (TODO: link to doc) that makes it so the controlled
/// entity will chase enemy entities in its field of view. Whenever the
/// controlled entity *starts* chasing an enemy entity, a signal will be
/// emitted. As long as the controlled entity *continues* to chase that entity, a
/// signal will continue to periodically be emitted in order to tell you the
/// updated distance between the controlled entity and the entity being chased.
///
/// Example usage:
/// ECChaseEnemies* c = new ECChaseEnemies(entity);
/// c->setStopDistance(50);
/// connect(c,&ECChaseEnemies::entityChaseStarted,this,myCallback);
/// connect(c,&ECChaseEnemies::entityChaseContinued,this,myCallback);
class ECChaseEnemies: public QObject
{
    Q_OBJECT
public:
    ECChaseEnemies(Entity* entity);
    ~ECChaseEnemies();

    void stopChasing();
    void startChasing();
    void setStopDistance(double distance);
    double stopDistance();

signals:
    /// Emitted whenever the controlled entity *starts* chasing an enemy entity.
    /// @param distToChasedEntity the distance between the controlled entity and the enemy
    /// entity that the controlled entity just started chasing
    void entityChaseStarted(Entity* chasedEntity, double distToChasedEntity);

    /// Emitted every once in a while to update you on the distance between the
    /// controlled entity and the entity being chased.
    /// @see entityChaseStarted()
    void entityChaseContinued(Entity* chasedEntity, double distToChasedEntity);

public slots:
    void onEntityEntersFOV_(Entity* entity);
    void onEntityLeavesFOV_(Entity* entity);
    void onEntityMoved_();

    void chaseStep_();

private:
    // options
    double stopDistance_;

    QPointer<Entity> entity_;
    ECFieldOfViewEmitter* fovEmitter_; // helper controller that emits events whenever
                                       // anothe entity enters/leaves the controlled entity's fov
    ECPathMover* pathMover_;
    QTimer* chaseTimer_;

    bool shouldChase_;
    QPointer<Entity> entityBeingChased_;
};

#endif // ECCHASEENEMIES_H
