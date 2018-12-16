#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QTimer;

namespace qge{

class ECPathMover;
class ECFieldOfViewEmitter;

/// An entity controller that makes it so the controlled entity will chase certain other entities
/// that enter its field of view.
///
/// Use addChasee() to add entities that should be chased when within the field of view.
///
/// Whenever the controlled entity *starts* chasing an entity, a signal (entityChaseStarted()) will
/// be emitted. Everytime the controlled entity takes a step closer to the chased entity another
/// signal (entityChaseContinued()) will be emitted. You can set how close the controlled entity
/// will get to the chased entity before it pauses ("stop distance"). When the controlled entity
/// gets to within the specified distance a signal (entityChasePaused()) will be emitted and the
/// controlled entity won't get any closer. When the chased entity gets out of the stop distance,
/// the controlled entity will start chasing again and entityChaseContinued() will be emitted.
///
/// Some relavent terminlogy used in the context of ECChaser:
/// - a "chasee" is any Entity that will be chased if it enters the field of view of the controlled entity.
/// - the "target" or "target chasee" entity is the chasee that the controller is *currently* chasing.
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

    void setShowFOV(bool tf);

signals:
    /// Emitted whenever the controlled entity *starts* chasing an entity.
    /// @param distToChasedEntity the distance between the controlled entity and the chased
    /// entity.
    void entityChaseStarted(Entity* chasedEntity, double distToChasedEntity);

    /// Emitted every time the controlled entity takes a step closer towards the chased entity.
    /// @see entityChaseStarted()
    void entityChaseContinued(Entity* chasedEntity, double distToChasedEntity);

    /// Emitted every time the controlled entity gets within the stop distance of the chased
    /// entity and thus stops.
    void entityChasePaused(Entity* chasedEntity);

public slots:
    void onEntityEntersFOV_(Entity* entity);
    void onEntityLeavesFOV_(Entity* entity);

    void onControlledEntityMoved_();

    void onStopRangeEntered_(Entity* watched, Entity* watching, double range);
    void onStopRangeLeft_(Entity* watched, Entity* watching, double range);

    void onControlledEntityDies_(QObject* entity);
    void onChasedEntityDies_(QObject* entity);

    void onControlledEntityLeavesMap_(Entity* entity);
    void onChasedEntityLeavesMap_(Entity* entity);

    void onChaseeDestroyed_(QObject* chasee);

    void onControlledEntityEntersMap_();

    void chaseStep_();

private:
    std::unordered_set<Entity*> chasees_;

    // options
    double stopDistance_;

    ECFieldOfViewEmitter* fovEmitter_; // helper controller that emits events whenever
                                       // another entity enters/leaves the controlled entity's fov
    ECPathMover* pathMover_;
    QTimer* chaseTimer_;

    bool paused_;   // controlled entity is w/i stop distance of chased entity
    QPointer<Entity> targetEntity_;

    void connectToTargetSignals_();
    void disconnectFromTargetSignals_();
};

}
