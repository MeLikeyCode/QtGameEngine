#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QTimer;

namespace qge{

class ECChaser;
class BodyThrust;

/// An entity controller that causes the controlled entity to chase certain other entites
/// in its field of view and use the BodyThrust ability on them when close enough.
///
/// Will chase both "target" entities and any entities that are enemies of the controlled entity.
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

    void addTargetEntity(Entity* entity);
    void removeTargetEntity(Entity* entity);
    std::unordered_set<Entity*> targetEntities() const;

    void setThrustDistance(double distance);
    void setAnimationToPlayWhileThrusting(const std::string& animationName);

    void setShowFOV(bool tf);

public slots:
    void onChaseContinued_(Entity* entityChased, double distance);
    void onChasePaused_(Entity* entity);
    void periodicCheck_();

signals:
    /// Emitted each time the controlled entity thrusts towards an enemy.
    void thrusted(Entity* towardsEnemy);

private:
    ECChaser* controllerChaseEnemies_;
    BodyThrust* bodyThrustAbility_;
    QPointer<Entity> lastEntityChased_;

    QTimer* periodicCheckTimer_;

    void bodyThrustIfCloseEnough_();
};

}
