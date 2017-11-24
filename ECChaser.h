#ifndef ECCHASEENEMIES_H
#define ECCHASEENEMIES_H

#include <QPointer>
#include <unordered_set>
#include <memory>

#include "EntityController.h"
#include "Entity.h"

class ECPathMover;
class ECFieldOfViewEmitter;
class QTimer;

/// An entity controller that makes it so the controlled
/// entity will chase certain other entities that enter its field of view. Whenever the
/// controlled entity *starts* chasing an entity, a signal will be
/// emitted. As long as the controlled entity *continues* to chase that entity, a
/// signal will continue to periodically be emitted in order to tell you the
/// updated distance between the controlled entity and the entity being chased.
/// You can set how close the controlled entity will get to the chased entity. When
/// the controlled entity gets to within the specified distance, it won't get any closer
/// unless the chased entity moves far again.
///
/// Some relavent terminlogy used in the context of ECChaser:
/// - a "chasee" is any Entity that will be chased if it enters the field ov view of the controlled entity
/// - the "target" entity is the entity that the controller is currently chasing
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECChaser* c = new ECChaser(entity);
/// c->addChasee(someOtherEntity);
/// c->addChasee(someOtherEntity2); // "Chasees" will be chased when they enter the controlled entity's field of view
/// c->setStopDistance(50); // set how close you want the controlled entity to get to the chased entity
/// connect(c,&ECChaser::entityChaseStarted,this,myCallback);
/// connect(c,&ECChaser::entityChaseContinued,this,myCallback);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ECChaser: public EntityController
{
    Q_OBJECT
public:
    ECChaser(Entity* entity);

    void addChasee(Entity* entity);
    void removeChasee(Entity* entity);
    std::unordered_set<Entity*> chasees() const;

    void stopChasing();
    void startChasing();
    void setStopDistance(double distance);
    double stopDistance() const;

signals:
    /// Emitted whenever the controlled entity *starts* chasing an entity.
    /// @param distToChasedEntity the distance between the controlled entity and the chased
    /// entity.
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
    void onChaseeDestroyed_(QObject* chasee);
    void chaseStep_();

private:
    std::unordered_set<Entity*> chasees_;

    // options
    double stopDistance_;

    ECFieldOfViewEmitter* fovEmitter_; // helper controller that emits events whenever
                                       // another entity enters/leaves the controlled entity's fov
    ECPathMover* pathMover_;
    QTimer* chaseTimer_;

    bool shouldChase_;
    bool paused_;   // controlled entity is w/i stop distance of chased entity
    QPointer<Entity> targetEntity_;
};

#endif // ECCHASEENEMIES_H
