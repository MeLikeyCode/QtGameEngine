#include "Projectile.h"
#include "Map.h"
#include "ProjectileMoveBehavior.h"
#include "CollisionBehavior.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test

Projectile::Projectile(QPointF start,
                       ProjectileMoveBehavior* moveBehavior,
                       CollisionBehavior *collisionBehavior,
                       Sprite* spr,
                       std::unordered_set<Entity *> noDamageList, Map *map):
    start_(start),
    moveBehavior_(moveBehavior),
    collisionBehavior_(collisionBehavior),
    noDamageList_(noDamageList),
    timer_(new QTimer(this))
{
    // make sure behaviors act on this projectile
    moveBehavior->projectile_ = this;

    setSprite(spr);     // set the sprite

    // default step frequency and size
    stepFrequency_ = 50;
    stepSize_ = 15;

    setPointPos(start);    // set the position of the Projectile to the start

    map->addEntity(this); // add the projectile to the map (could alternatively use setMap(map));
}

QPointF Projectile::start()
{
    return start_;
}

void Projectile::setStart(QPointF start)
{
    start_ = start;
}

int Projectile::stepFrequency()
{
    return stepFrequency_;
}

void Projectile::setStepFrequency(int f)
{
    stepFrequency_ = f;
}

void Projectile::addToNoDamageList(Entity *entity)
{
    noDamageList_.insert(entity);
}

bool Projectile::isInNoDamageList(Entity *entity)
{
    return noDamageList_.count(entity) > 0;
}

/// Returns a list of entities that the projectile is currently colliding with
/// (at its current position).
std::unordered_set<Entity*> Projectile::collidingEntities()
{
    std::unordered_set<Entity*> entities = map()->entities(this->pointPos(),pointZ(),pointZ()+height());
    entities.erase(this);     // make sure the projectile itself is not in this list

    return entities;
}

/// Returns the ProjectileMoveBehavior of the Projectile.
ProjectileMoveBehavior *Projectile::moveBehavior()
{
    return moveBehavior_.get();
}

/// Sets the ProjectileMoveBehavior of the Projectile.
void Projectile::setMoveBehavior(ProjectileMoveBehavior *moveBehavior)
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

void Projectile::startMoving()
{
    connect(timer_,&QTimer::timeout,this,&Projectile::step_);
    timer_->start(stepFrequency());
}

void Projectile::setStepSize(int size)
{
    stepSize_ = size;
}

int Projectile::stepSize()
{
    return stepSize_;
}

std::unordered_set<Entity *> Projectile::noDamageList()
{
    return noDamageList_;
}

void Projectile::setNoDamageList(std::unordered_set<Entity *> noDamageList)
{
    noDamageList_ = noDamageList;
}

/// Executed every "step" the projectile needs to take.
/// First the MoveBehavior is called which determines how it should move.
/// After moving, a collision check is done, if the projectile collides with
/// any Entities, the CollisionBehavior will be notified.
/// The projectile is passed in as "entityOne" for the CollisionBehavior
/// while the colliding entities are passed in as "entityTwo."
void Projectile::step_()
{
    // call move behavior
    moveBehavior_->onMoveStep();

    // call collision behavior (passing all collided entities)
    std::unordered_set<Entity*> cEntities = collidingEntities();
    for (Entity* entity:cEntities){
        collisionBehavior_->onCollided(this,entity);
    }
}
