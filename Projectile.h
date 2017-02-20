#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <unordered_set>
#include "Entity.h"
#include "Mover.h"
#include "CollisionBehavior.h"
#include <memory>
#include <QPointer>
#include "DestReachedBehavior.h"

class QTimer;

/// Represents a projectile that moves a certain way and collides with things
/// along the way.
///
/// Strategy pattern is used to determine the behavior of the projectile. The
/// Mover of the Projectile determines how the projectile moves. The
/// CollisionBehavior determines how the projectile responds when it collides
/// with Entities. The DestReachedBehavior determines what happens when the
/// Projectile reaches its destination.
///
/// To shoot a projectile towards a certain pos use shootTowards(). To have the
/// projectile "home" to an Entity (i.e. follow the Entity while traveling),
/// use homeTowards(). homeTowards() simply periodically calls shootTowards()
/// passing in the new position of the Entity.
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Projectile* p = new SomeConcreteProjectileClass(aMover, aCollisionBehavior, listOfEntitiesToNotDmg);
/// p->setPointPos(somePos); // set the position of the projectile
/// p->shootTowards(someOtherPos); // shoot the projectile towards the specified pos
/// p->homeTowards(someEntity); // periodically redirects the projectile to follow said entity
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// How to create your own custom Projectiles
/// =========================================
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
    Projectile(Mover *mover = nullptr,
               CollisionBehavior *collisionBehavior = nullptr,
               DestReachedBehavior* destReachedBeahvior = nullptr);

    // shooting
    virtual void shootTowards(const QPointF& pos);
    void homeTowards(Entity* entity);

    // behaviors
    Mover* moveBehavior();
    void setMover(Mover* mover);

    CollisionBehavior *collisionBehavior();
    void setCollisionBehavior(CollisionBehavior *collisionBehavior);

    DestReachedBehavior* destReachedBehavior();
    void setDestReachedBehavior(DestReachedBehavior* drb);

public slots:
    void onCollided_(Entity* self, Entity* collidedWith);
    void onSuccesfullyMoved_(Mover* byMover);
    void onHomeStep_();

private:
    // behaviors
    std::unique_ptr<Mover> mover_;
    std::unique_ptr<CollisionBehavior> collisionBehavior_;
    std::unique_ptr<DestReachedBehavior> destReachedBehavior_;

    QPointer<Entity> homeTo_;
    QTimer* homeTimer_;
};

#endif // PROJECTILE_H
