#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <unordered_set>
#include "Entity.h"
#include "Mover.h"
#include "CollisionBehavior.h"
#include <memory>
#include <QPointer>

class QTimer;

/// Represents a projectile that moves a certain way and collides with things
/// along the way. Strategy pattern is used to determine the behaviors of the projectile.
/// Projectiles have 2 behaviors:
/// - MoveBehavior
/// - CollisionBehavior
///
/// The MoveBehavior determines how the projectile moves.
///
/// The CollisionBehavior determines how the projectile responds when
/// it collides with Entities.
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
/// To move shoot a projectile towards a certain pos use shootTowards(). To have the
/// projectile "home" to an Entity (i.e. follow the Entity while traveling), use
/// homeTowards(). homeTowards() simply periodically calls shootTowards() passing in
/// the new position of the Entity.
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Projectile* p = new SomeConcreteProjectileClass(aMoveBehavior, aCollisionBehavior, listOfEntitiesToNotDmg);
/// p->setPointPos(somePos); // set the position of the projectile
/// p->shootTowards(someOtherPos); // shoot the projectile towards the specified pos
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile: public Entity
{
    Q_OBJECT
public:
    Projectile(Mover *moveBehavior,
               CollisionBehavior *collisionBehavior,
               std::unordered_set<Entity*> noDamageList);

    void shootTowards(const QPointF& pos);
    void homeTowards(Entity* entity);

    std::unordered_set<Entity*> noDamageList();
    void setNoDamageList(std::unordered_set<Entity*> noDamageList);
    void addToNoDamageList(Entity* entity);
    bool isInNoDamageList(Entity* entity);

    Mover* moveBehavior();
    void setMoveBehavior(Mover* moveBehavior);

    CollisionBehavior *collisionBehavior();
    void setCollisionBehavior(CollisionBehavior *collisionBehavior);

public slots:
    void onCollided_(Entity* self, Entity* collidedWith);
    void onHomeStep_();

private:
    std::unordered_set<Entity*> noDamageList_;

    // behaviors
    std::unique_ptr<Mover> moveBehavior_;
    std::unique_ptr<CollisionBehavior> collisionBehavior_;

    QPointer<Entity> homeTo_;
    QTimer* homeTimer_;
};

#endif // PROJECTILE_H
