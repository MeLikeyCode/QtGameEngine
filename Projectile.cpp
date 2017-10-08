#include "Projectile.h"
#include "Map.h"
#include "CollisionBehavior.h"
#include <QTimer>
#include <QDebug> // TODO: remove, test
#include <cassert>

/// Constructs a Project from the specified behaviors.
/// You can pass in null for any of the behaviors, but be sure to use the setters
/// to set all the behaviors prior to calling shootTowards() or homeTowards().
Projectile::Projectile(ECMover *mover,
                       CollisionBehavior *collisionBehavior,
                       DestReachedBehavior* destReachedBehavior):
    mover_(mover),
    collisionBehavior_(collisionBehavior),
    destReachedBehavior_(destReachedBehavior)
{
    setMover(mover);

    // listen to when projectile collides with anything
    connect(this, &Entity::collided,this,&Projectile::onCollided_);
}

/// Returns the MoveBehavior of the Projectile.
ECMover *Projectile::moveBehavior()
{
    return mover_.get();
}

/// Sets the Mover of the Projectile.
/// Destroys the old Mover.
/// Please ensure that the Projectile has a Mover before calling shootTowards()
/// or homeTowards().
void Projectile::setMover(ECMover *mover)
{
    mover_ .reset(mover);

    if (mover == nullptr)
        return;

    // make sure Mover operates on this entity
    mover_->setEntity(this);

    // listen to when the projectile is done moving
    connect(mover, &ECMover::entitySuccesfullyMoved, this, &Projectile::onSuccesfullyMoved_);

}

/// Returns the CollisionsBehvaior of the Projectile.
CollisionBehavior *Projectile::collisionBehavior()
{
    return collisionBehavior_.get();
}

/// Sets the CollisionBehavior of the Projectile.
/// Destroys the old CollisionBehavior.
void Projectile::setCollisionBehavior(CollisionBehavior *collisionBehavior)
{
    collisionBehavior_.reset(collisionBehavior);
}

/// Returns the DestReachedBeahvior of the Projectile.
DestReachedBehavior *Projectile::destReachedBehavior()
{
    return destReachedBehavior_.get();
}

/// Sets the DestReachedBeahvior of the Projectile.
/// Destroys the old DestReachedBehavior.
void Projectile::setDestReachedBehavior(DestReachedBehavior *drb)
{
    destReachedBehavior_.reset(drb);
}

/// Executed when the Projectile collides with something.
/// Will delegate what happens to the CollisionBehavior.
void Projectile::onCollided_(Entity *self, Entity *collidedWith)
{
    Q_UNUSED(self);
    collisionBehavior_->onCollided(this, collidedWith);
}

/// Executed when the Projectile has finished its moving (has reached its final
/// destination). Will let the DestReachedBehavior know.
void Projectile::onSuccesfullyMoved_(ECMover *byMover)
{
    Q_UNUSED(byMover);
    destReachedBehavior_->onDestinationReached(*this);
}

/// Executed periodically when homeTowards() was used.
/// Will simply re-shoot the Projectile towards the Entity.
void Projectile::onHomeStep_()
{
    shootTowards(homeTo_->pos());
}

/// Shoots the Projectile towards the specified position.
/// @warning Please make sure that the behaviors are set before calling this function.
void Projectile::shootTowards(const QPointF &pos)
{
    // assert that behaviors are set
    assert(mover_ != nullptr);
    assert(collisionBehavior_ != nullptr);
    assert(destReachedBehavior_ != nullptr);

    mover_->moveEntity(pos);
}

/// Homes the projectile towards the specified Entity.
/// This makes it so that the projectile "follows" the Entity (i.e. won't miss it).
/// @warning Please make sure that the behaviors are set before calling this function.
void Projectile::homeTowards(Entity *entity)
{
    // assert that behaviors are set
    assert(mover_ != nullptr);
    assert(collisionBehavior_ != nullptr);
    assert(destReachedBehavior_ != nullptr);

    homeTo_ = entity;
    homeTimer_->disconnect();
    connect(homeTimer_,&QTimer::timeout,this,&Projectile::onHomeStep_);
    const int HOME_FREQ = 50; // how often to keep changing direction to follow player
    homeTimer_->start(HOME_FREQ);
}
