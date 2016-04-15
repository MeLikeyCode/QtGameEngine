#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include <QObject>
#include <QPointF>
#include <unordered_set>
#include <QTimer>

class ProjectileMoveBehavior;
class ProjectileCollisionBehavior;
class ProjectileRangeReachedBehavior;

/// Represents a projectile that moves a certain distance.
/// Strategy pattern is used to determine the behaviors of the projectile.
/// Projectiles have 3 behaviors:
/// -ProjectileMoveBehavior
/// -ProjectileCollisionBehavior
/// -ProjectileRangeReachedBehavior
///
/// The ProjectileMoveBehavior determines how the projectile moves. It has a
/// function that is executed each time the projectile is asked to move. This
/// function should move the projectile and then return how much it has moved.
/// (The amount moved is used to determine when a projectile has reached its range)
///
/// The ProjectileCollisionBehavior determines how the projectile responds when
/// it collides with Entities.
///
/// The ProjectileRangeReachedBehavior determines what the projectile does when
/// it has reached its max range (i.e. it has traveled as far as it was supposed
/// to).
///
/// In order to create your own Projectiles, you should sublcass one or more of the
/// behaviors to create your own behaviors. Then simply construct a projectile and
/// give them your own behaviors :).
///
/// Note that several prebuilt behaviors are included.
///
/// Every projectile maintains a list of entities that it should not damage.
/// The Entity that spawns a projectile should be adde to this list to prevent
/// the projectile from damaging him (addToNoDamageList()).
///
/// IMPORTANT: After creating a projectile, invoke the start() method to get it
/// to start moving.
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile: public Entity
{
    Q_OBJECT
public:
    Projectile(ProjectileMoveBehavior *moveBehavior,
               ProjectileCollisionBehavior *collisionBehavior,
               ProjectileRangeReachedBehavior *rangeReachedBehavior);

    void go(QPointF start, QPointF targetPoint, double range);

    QPointF start();
    void setStart(QPointF start);

    QPointF targetPoint();
    void setTargetPoint(QPointF target);

    double range();
    void setRange(double range);

    double distanceTravelled();

    int stepFrequency();
    void setStepFrequency(int f);

    void setStepSize(int size);
    int stepSize();

    void addToNoDamageList(Entity* entity);
    bool isInNoDamageList(Entity* entity);

    std::unordered_set<Entity*> collidingEntities();

public slots:
    void step_();

private:
    QPointF targetPoint_;
    QPointF start_;
    double range_;
    double distanceMoved_;
    std::unordered_set<Entity*> noDamageList_;
    int stepFrequency_;
    int stepSize_;
    QTimer* timer_;

    // behaviors
    ProjectileMoveBehavior* moveBehavior_;
    ProjectileCollisionBehavior* collisionBehavior_;
    ProjectileRangeReachedBehavior* rangeReachedBehavior_;


};

#endif // PROJECTILE_H
