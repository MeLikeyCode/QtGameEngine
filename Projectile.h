#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QPointF>
#include <unordered_set>
#include <QTimer>
#include <QPointer>
#include "Entity.h"

class ProjectileMoveBehavior;
class ProjectileCollisionBehavior;

/// Represents a projectile that moves a certain distance and collides with things
/// along the way. Strategy pattern is used to determine the behaviors of the projectile.
/// Projectiles have 2 behaviors:
/// -ProjectileMoveBehavior
/// -ProjectileCollisionBehavior
///
/// The ProjectileMoveBehavior determines how the projectile moves. It has a
/// function that is executed each time the projectile is asked to move. This
/// function should move the projectile in its own way (straight, squiggily, etc...).
///
/// The ProjectileCollisionBehavior determines how the projectile responds when
/// it collides with Entities. It has a function that recieves what it has collided
/// with, the function should respond accordingly (weather it damages, heals, etc..).
///
/// In order to create your own Projectiles, you should sublcass one or more of the
/// behaviors to create your own behaviors. Then simply construct a projectile and
/// give them your own behaviors :).
///
/// Note that several prebuilt behaviors are included.
///
/// Every projectile maintains a list of entities that it should not damage.
/// The Entity that spawns a projectile should be added to this list to prevent
/// the projectile from damaging him (addToNoDamageList(Entity*)).
///
/// A Projectile starts moving immediately after it's constructed.
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile: public Entity
{
    Q_OBJECT
public:
    Projectile(QPointF start,
               ProjectileMoveBehavior *moveBehavior,
               ProjectileCollisionBehavior *collisionBehavior);

    ~Projectile();

    QPointF start();

    int stepFrequency();
    void setStepFrequency(int f);

    void setStepSize(int size);
    int stepSize();

    void addToNoDamageList(Entity* entity);
    bool isInNoDamageList(Entity* entity);

    std::unordered_set<QPointer<Entity>> collidingEntities();
public slots:
    void step_();

private:
    QPointF start_;
    std::unordered_set<Entity*> noDamageList_;
    int stepFrequency_;
    int stepSize_;
    QTimer* timer_;

    // behaviors
    ProjectileMoveBehavior* moveBehavior_;
    ProjectileCollisionBehavior* collisionBehavior_;
};

#endif // PROJECTILE_H
