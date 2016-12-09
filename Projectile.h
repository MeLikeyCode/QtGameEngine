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

/// Represents a projectile that moves a certain way and collides with things
/// along the way. Strategy pattern is used to determine the behaviors of the projectile.
/// Projectiles have 2 behaviors:
/// -ProjectileMoveBehavior
/// -ProjectileCollisionBehavior
///
/// The ProjectileMoveBehavior determines how the projectile moves. It has a
/// function that is executed each time the projectile is asked to move. This
/// function moves the projectile in its own way (straight, squiggily, etc...).
/// To set how often the projectile is asked to move (and therefore how often the
/// ProjectileMoveBehavior is called) use set setStepFrequency().
///
/// The ProjectileCollisionBehavior determines how the projectile responds when
/// it collides with Entities. It has a function that recieves what it has collided
/// with, the function responds accordingly (weather it damages, heals, etc..).
///
/// In order to create your own Projectiles, you should sublcass one or more of the
/// behaviors to create your own behaviors. Then simply construct a projectile and
/// give them your own behaviors :).
///
/// Note that several prebuilt behaviors are included as well as several concrete
/// Projectile subclasses that come with standard move and collisions behaviors
/// (see SpearProjectile).
///
/// Every projectile maintains a collection of entities that it should not damage.
/// The Entity that spawns a projectile should probably be added to this list to prevent
/// the projectile from damaging him (unless that is the intended effect).
///
/// To make the projectile start moving call its startMoving() function.
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile: public Entity
{
    Q_OBJECT
public:
    Projectile(QPointF start,
               ProjectileMoveBehavior *moveBehavior,
               ProjectileCollisionBehavior *collisionBehavior,
               Sprite *spr,
               std::unordered_set<Entity*> noDamageList,
               Map* map);

    ~Projectile();

    QPointF start();
    void setStart(QPointF start);

    int stepFrequency();
    void setStepFrequency(int f);

    void setStepSize(int size);
    int stepSize();

    std::unordered_set<Entity*> noDamageList();
    void setNoDamageList(std::unordered_set<Entity*> noDamageList);
    void addToNoDamageList(Entity* entity);
    bool isInNoDamageList(Entity* entity);

    std::unordered_set<QPointer<Entity>> collidingEntities();

    ProjectileMoveBehavior* moveBehavior();
    void setMoveBehavior(ProjectileMoveBehavior* moveBehavior);

    ProjectileCollisionBehavior* collisionBehavior();
    void setCollisionBehavior(ProjectileCollisionBehavior* collisionBehavior);

    void startMoving();
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
