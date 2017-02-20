#include "Projectile.h"
#include "Map.h"
#include "CollisionBehavior.h"
#include <QTimer>
#include <QDebug> // TODO: remove, test
#include <cassert>

/// Constructs a Project from the specified behaviors and no damage list.
/// You can pass in null for any of the behaviors, but be sure to use the setters
/// to set all the behaviors prior to calling shootTowards() or homeTowards().
Projectile::Projectile(Mover *mover,
                       CollisionBehavior *collisionBehavior,
                       DestReachedBehavior* destReachedBehavior,
                       std::unordered_set<Entity *> noDamageList):
    noDamageList_(noDamageList),
    mover_(mover),
    collisionBehavior_(collisionBehavior),
    destReachedBehavior_(destReachedBehavior)
{
    // make sure behaviors act on the projectile
    setMover(mover);

    // listen to when projectile collides with anything
    connect(this, &Entity::collided,this,&Projectile::onCollided_);

    // listen to when the projectile is done moving
    connect(moveBehavior, &Mover::entitySuccesfullyMoved, this, &Projectile::onSuccesfullyMoved_);
}

void Projectile::addToNoDamageList(Entity *entity)
{
    noDamageList_.insert(entity);
}

bool Projectile::isInNoDamageList(Entity *entity)
{
    return noDamageList_.count(entity) > 0;
}

/// Returns the MoveBehavior of the Projectile.
Mover *Projectile::moveBehavior()
{
    return mover_.get();
}

/// Sets the Mover of the Projectile.
/// Destroys the old Mover.
void Projectile::setMover(Mover *moveBehavior)
{
    mover_ .reset(moveBehavior);
    mover_->setEntity(this);
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
void Projectile::onSuccesfullyMoved_(Mover *byMover)
{
    Q_UNUSED(byMover);
    destReachedBehavior_->onDestinationReached(*this);
}

/// Executed whenever the Projectile is asked to home in on an Entity.
/// Will simply shoot towards the Entity.
void Projectile::onHomeStep_()
{
    shootTowards(homeTo_->pointPos());
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
    homeTo_ = entity;
    homeTimer_->disconnect();
    connect(homeTimer_,&QTimer::timeout,this,&Projectile::onHomeStep_);
    const int HOME_FREQ = 50; // how often to keep changing direction to follow player
    homeTimer_->start(HOME_FREQ);
}

std::unordered_set<Entity *> Projectile::noDamageList()
{
    return noDamageList_;
}

void Projectile::setNoDamageList(std::unordered_set<Entity *> noDamageList)
{
    noDamageList_ = noDamageList;
}
