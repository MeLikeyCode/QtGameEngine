#ifndef ECCHASEENEMIES_H
#define ECCHASEENEMIES_H

#include <QObject>
#include <QPointer>
#include <unordered_set>
#include "Entity.h"

class ECPathMover;
class ECFieldOfViewEmitter;
class QTimer;

/// An entity controller (TODO: link to doc) that makes it so an entity will
/// chase enemy entities in its field of view. Whenever the controlled entity
/// starts chasing a new enemy entity, a signal will be emitted. As long as the
/// controlled entity is chasing some enemy entity, ECChaseEnemies will continue
/// to periodically emit a signal telling you the distance between the controlled
/// entity and the entity being chased.
///
/// Example usage:
/// ECChaseEnemies* c = new ECChaseEnemies(entity);
/// c->addEnemyGroup(4);
/// connect(c,&ECChaseEnemies::entityChaseStarted,this,myCallback);
/// connect(c,&ECChaseEnemies::entityChaseContinued,this,myCallback);
class ECChaseEnemies: public QObject
{
    Q_OBJECT
public:
    ECChaseEnemies(Entity* entity);

    void addEnemyGroup(int groupNumber);
    std::unordered_set<int> enemyGroups();
    void stopChasing();
    void startChasing();
    bool isAnEnemyGroup(int groupNumber);

signals:
    /// Emitted whenever the controlled entity *starts* chasing an enemy entity.
    /// @param range the distance between the controlled entity and the enemy
    /// entity that the controlled entity just started chasing
    void entityChaseStarted(Entity* entity, double range);

    /// Emitted every once in a while to update you on the distance between the
    /// controlled entity and the entity being chased.
    /// @see entityChaseStarted()
    void entityChaseContinued(Entity* entity, double range);

public slots:
    void onEntityEntersFOV_(Entity* entity);
    void onEntityLeavesFOV_(Entity* entity);
    void onEntityMoved_();

    void chaseStep_();

private:
    QPointer<Entity> entity_;
    std::unordered_set<int> enemyGroups_;

    ECFieldOfViewEmitter* fovEmitter_; // helper controller that emits events whenever
                                       // anothe entity enters/leaves the controlled entity's fov
    ECPathMover* pathMover_;

    QTimer* chaseTimer_;

    bool shouldChase_;
    QPointer<Entity> entityBeingChased_;
};

#endif // ECCHASEENEMIES_H
