#ifndef ECCHASEENEMIES_H
#define ECCHASEENEMIES_H

#include <QObject>
#include <QPointer>
#include <unordered_set>
#include "Entity.h"
#include <memory>

class PathMover;
class ECFieldOfViewEmitter;
class QTimer;

/// An entity controller (TODO: link to doc) that makes it so the controlled
/// entity will chase enemy entities in its field of view. Whenever the
/// controlled entity *starts* chasing an enemy entity, a signal will be
/// emitted. As long as the controlled entity *continues* to chase that entity, a
/// signal will continue to periodically be emitted in order to tell you the
/// updated distance between the controlled entity and the entity being chased.
/// You can set how close the controlled entity will get to the chased entity. When
/// the controlled entity gets to within the specified distance, it won't get any closer
/// unless the chased entity moves far again.
///
/// Example usage:
/// ECChaseEnemies* c = new ECChaseEnemies(entity);
/// c->setStopDistance(50); // set how close you want the controlled entity to get to the chased entity
/// connect(c,&ECChaseEnemies::entityChaseStarted,this,myCallback);
/// connect(c,&ECChaseEnemies::entityChaseContinued,this,myCallback);
class ECChaseEnemies: public QObject
{
    Q_OBJECT
public:
    ECChaseEnemies(Entity& entity);

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
    void onEntityEntersRange_(Entity* watched, Entity* watching, double range);
    void onEntityLeavesRange_(Entity* watched, Entity* watching, double range);
    void onChasingEntityDies_(QObject* entity);
    void onChasedEntityDies_(QObject* entity);
    void onChasingEntityLeavesMap_(Entity* entity);
    void onChasedEntityLeavesMap_(Entity* entity);
    void chaseStep_();

private:
    // options
    double stopDistance_;

    QPointer<Entity> controlledEntity_;
    std::unique_ptr<ECFieldOfViewEmitter> fovEmitter_; // helper controller that emits events whenever
                                                       // anothe entity enters/leaves the controlled entity's fov
    std::unique_ptr<PathMover> pathMover_;
    QTimer* chaseTimer_;

    bool shouldChase_;
    bool paused_;   // controlled entity is w/i stop distance of chased entity
    QPointer<Entity> targetEntity_;
};

#endif // ECCHASEENEMIES_H
