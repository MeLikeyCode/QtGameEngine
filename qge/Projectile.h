#pragma once

#include "Vendor.h"

#include "Entity.h"
#include "ECMover.h"
#include "CollisionBehavior.h"
#include "DestReachedBehavior.h"

class QTimer;

namespace qge{

/// An Entity that represents a projectile that moves a certain way and collides with things
/// along the way.
///
/// Strategy pattern is used to determine the behavior of the projectile. The
/// ECMover of the Projectile determines how the projectile moves. The
/// CollisionBehavior determines how the projectile responds when it collides
/// with Entities. The DestReachedBehavior determines what happens when the
/// Projectile reaches its destination. If you don't want the projectile
/// to collide with a certain group of entities, give that group a tag, and then
/// call addEntitiesToNotCollideWith() passing in that tag. If you want the projectile to
/// collide but not damage a certain group of entities, give that group a tag and
/// call addEntitiesToNotDamage() passing in that tag.
///
/// To shoot a projectile towards a certain pos use shootTowards(). To have the
/// projectile "home" to an Entity (i.e. follow the Entity while traveling),
/// use homeTowards(). homeTowards() simply periodically calls shootTowards()
/// passing in the new position of the Entity.
///
/// Example usage
/// -------------
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Projectile* p = new SomeConcreteProjectileClass(aMover, aCollisionBehavior, listOfEntitiesToNotDmg);
/// p->addEntitiesToNotCollideWith("ghost"); // pass through all entities tagged with "ghost"
/// p->addEntitiesToNotDamage("good guy"); // collide with but do not damage all entities tagged with "good guy"
/// p->setPos(somePos); // set the position of the projectile
/// p->shootTowards(someOtherPos); // shoot the projectile towards the specified pos
/// p->homeTowards(someEntity); // periodically redirects the projectile to follow said entity
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// How to create your own custom Projectiles
/// -----------------------------------------
/// To create your own custom projectile, you simply need to create your own custom
/// Mover, CollisionBehavior, and DestReachedBehavior. Then give your projectile
/// these behaviors.
///
/// Note that several prebuilt Movers/CollisionBehaviors/DestReachedBehaviors
/// are included as well as several concrete Projectile subclasses that you can
/// use as examples to follow.
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile: public Entity
{
    Q_OBJECT
public:
    Projectile(ECMover *mover = nullptr,
               CollisionBehavior *collisionBehavior = nullptr,
               DestReachedBehavior* destReachedBeahvior = nullptr);

    // shooting
    virtual void shootTowards(const QPointF& pos);
    void homeTowards(Entity* entity);

    // behaviors
    ECMover* moveBehavior();
    void setMover(ECMover* mover);

    CollisionBehavior *collisionBehavior();
    void setCollisionBehavior(CollisionBehavior *collisionBehavior);

    DestReachedBehavior* destReachedBehavior();
    void setDestReachedBehavior(DestReachedBehavior* drb);

    void addEntitiesToNotCollideWith(const std::string& tag);
    void addEntityToNotCollideWith(Entity* entity);
    void addEntitiesToNotDamage(const std::string& tag);
    void addEntityToNotDamage(Entity* entity);

public slots:
    void onCollided_(Entity* self, Entity* collidedWith);
    void onSuccesfullyMoved_(ECMover* byMover);
    void onHomeStep_();

private:
    // behaviors
    ECMover* mover_;
    std::unique_ptr<CollisionBehavior> collisionBehavior_;
    std::unique_ptr<DestReachedBehavior> destReachedBehavior_;


    std::set<std::string> doNotCollideTags_; // ignore collisions with entities that have these tags
    std::set<std::string> doNotDamageTags_; // collide with but do not damage these entities
    std::set<Entity*> doNotCollideEntities_; // ignore collisions with these entities
    std::set<Entity*> doNotDamageEntities_; // collide with but do not damage these entites

    QPointer<Entity> homeTo_;
    QTimer* homeTimer_;
};

}
