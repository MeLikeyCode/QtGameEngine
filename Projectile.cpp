#include "Projectile.h"
#include "Map.h"
#include "ProjectileMoveBehavior.h"
#include "ProjectileCollisionBehavior.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test

Projectile::Projectile(QPointF start,
                       ProjectileMoveBehavior* moveBehavior,
                       ProjectileCollisionBehavior* collisionBehavior,
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
    collisionBehavior->projectile_ = this;

    setSprite(spr);     // set the sprite

    // default step frequency and size
    stepFrequency_ = 50;
    stepSize_ = 15;

    setPointPos(start);    // set the position of the Projectile to the start

    map->addEntity(this); // add the projectile to the map (could alternatively use setMap(map));
}

Projectile::~Projectile()
{
    delete timer_;
    delete moveBehavior_;
    delete collisionBehavior_;
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
std::unordered_set<QPointer<Entity>> Projectile::collidingEntities()
{
    std::unordered_set<Entity*> entities = map()->entities(this->pointPos(),pointZ(),pointZ()+height());
    entities.erase(this);     // make sure the projectile itself is not in this list

    // return QPointer<Entity>s so that they can be checked for null
    std::unordered_set<QPointer<Entity>> ePointers;
    for (Entity* e:entities){
        QPointer<Entity> p = e;
        ePointers.insert(p);
    }

    return ePointers;
}

/// Returns the ProjectileMoveBehavior of the Projectile.
ProjectileMoveBehavior *Projectile::moveBehavior()
{
    return moveBehavior_;
}

/// Sets the ProjectileMoveBehavior of the Projectile.
void Projectile::setMoveBehavior(ProjectileMoveBehavior *moveBehavior)
{
    moveBehavior_ = moveBehavior;
    moveBehavior_->projectile_ = this;
}

/// Returns the ProjectileCollisionsBehvaior of the Projectile.
ProjectileCollisionBehavior *Projectile::collisionBehavior()
{
    return collisionBehavior_;
}

/// Sets the ProjectileCollisionBehavior of the Projectile.
void Projectile::setCollisionBehavior(ProjectileCollisionBehavior *collisionBehavior)
{
    collisionBehavior_ = collisionBehavior;
    collisionBehavior_->projectile_ = this;
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
/// First the moveBehavior is called which determines how it should move.
/// (we keep track of how much it has moved in order to be able to tell when
/// it has reached its range).
/// After moving, a collision check is done, if the projectile collides with
/// any Entities, the collisionBehavior will be notified.
/// Third, if the Projectile has reached its range, the rangeReachedBehavior
/// will be notified.
void Projectile::step_()
{
    // call move behavior
    moveBehavior_->onMoveStep();

    // call collision behavior (passing all collided entities)
    std::unordered_set<QPointer<Entity>> cEntities = collidingEntities();
    if (cEntities.size() > 0){
        this->collisionBehavior_->onCollisionWith(cEntities);
    }
}
