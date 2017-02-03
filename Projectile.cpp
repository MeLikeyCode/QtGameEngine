#include "Projectile.h"
#include "Map.h"
#include "ProjectileMoveBehavior.h"
#include "CollisionBehavior.h"
#include <QTimer>
#include <QDebug> // TODO: remove, test

Projectile::Projectile(Mover *moveBehavior,
                       CollisionBehavior *collisionBehavior,
                       std::unordered_set<Entity *> noDamageList):
    moveBehavior_(moveBehavior),
    collisionBehavior_(collisionBehavior),
    noDamageList_(noDamageList)
{
    // make sure behaviors act on the projectile
    moveBehavior_->setEntity(this);

    // listen to when projectile collides with anything
    connect(this, &Entity::collided,this,&Projectile::onCollided_);
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
    return moveBehavior_.get();
}

/// Sets the MoveBehavior of the Projectile.
void Projectile::setMoveBehavior(Mover *moveBehavior)
{
    // release old move behavior
    moveBehavior_.release();

    // grab new one
    moveBehavior_ .reset(moveBehavior);
    moveBehavior_->projectile_ = this;
}

/// Returns the CollisionsBehvaior of the Projectile.
CollisionBehavior *Projectile::collisionBehavior()
{
    return collisionBehavior_.get();
}

/// Sets the CollisionBehavior of the Projectile.
void Projectile::setCollisionBehavior(CollisionBehavior *collisionBehavior)
{
    // remove old collision behavior
    collisionBehavior_.release();

    // grab new one
    collisionBehavior_.reset(collisionBehavior);
}

/// Executed when the Projectile collides with something.
/// Will delegate what happens to the CollisionBehavior.
void Projectile::onCollided_(Entity *self, Entity *collidedWith)
{
    collisionBehavior_->onCollided(this, collidedWith);
}

/// Executed whenever the Projectile is asked to home in on an Entity.
/// Will simply shoot towards the Entity.
void Projectile::onHomeStep_()
{
    shootTowards(homeTo_->pointPos());
}

/// Shoots the Projectile towards the specified position.
void Projectile::shootTowards(const QPointF &pos)
{
    moveBehavior_->moveTo(pos);
}

/// Homes the projectile towards the specified Entity.
/// This makes it so that the projectile "follows" the Entity (i.e. won't miss it).
void Projectile::homeTowards(Entity *entity)
{
    homeTo_ = entity;
    homeTimer_.disconnect();
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
