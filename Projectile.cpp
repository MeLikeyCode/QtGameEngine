#include "Projectile.h"
#include "Map.h"
#include "ProjectileMoveBehavior.h"
#include "ProjectileCollisionBehavior.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test

Projectile::Projectile(QPointF start, ProjectileMoveBehavior* moveBehavior, ProjectileCollisionBehavior* collisionBehavior):
    start_(start),
    moveBehavior_(moveBehavior),
    collisionBehavior_(collisionBehavior),
    timer_(new QTimer(this))
{
    // make sure behaviors act on this projectile
    moveBehavior->projectile_ = this;
    collisionBehavior->projectile_ = this;

    // default sprite
    Sprite* spr_ = new Sprite();
    QPixmap pm_ = QPixmap(":resources/graphics/weapons/spear.png");
    //pm_ = pm_.scaled(100,100);
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // defaults params
    stepFrequency_ = 50;
    stepSize_ = 15;

    setPointPos(start);    // set the position of the Projectile to start

    // start moving
    connect(timer_,&QTimer::timeout,this,&Projectile::step_);
    timer_->start(stepFrequency());
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
    std::unordered_set<Entity*> entities = map()->entities(this->pointPos());
    entities.erase(this);     // make sure the projectile itself is not in this list

    // return QPointer<Entity>s so that they can be checked for null
    std::unordered_set<QPointer<Entity>> ePointers;
    for (Entity* e:entities){
        QPointer<Entity> p = e;
        ePointers.insert(p);
    }

    return ePointers;
}

void Projectile::setStepSize(int size)
{
    stepSize_ = size;
}

int Projectile::stepSize()
{
    return stepSize_;
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
