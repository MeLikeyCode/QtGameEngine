#include "Projectile.h"
#include "Map.h"
#include "ProjectileMoveBehavior.h"
#include "ProjectileCollisionBehavior.h"
#include "ProjectileRangeReachedBehavior.h"

Projectile::Projectile(ProjectileMoveBehavior* moveBehavior, ProjectileCollisionBehavior* collisionBehavior, ProjectileRangeReachedBehavior* rangeReachedBehavior)
{
    this->moveBehavior_ = moveBehavior;
    moveBehavior->projectile_ = this;
    this->collisionBehavior_ = collisionBehavior;
    collisionBehavior->projectile_ = this;
    this->rangeReachedBehavior_ = rangeReachedBehavior;
    rangeReachedBehavior->projectile_ = this;

    // default sprite
    Sprite* spr_ = new Sprite();
    QPixmap pm_ = QPixmap(":resources/graphics/weapons/axe.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // defaults
    stepFrequency_ = 50;
    stepSize_ = 15;
    distanceMoved_ = 0;
    range_ = 500;
}

/// Start moving the projectile.
void Projectile::go(QPointF start, QPointF targetPoint, double range)
{
    setStart(start);
    setPointPos(start);
    setTargetPoint(targetPoint);
    setRange(range);

    // connect timer
    timer_ = new QTimer(this);
    connect(timer_,SIGNAL(timeout()),this,SLOT(step_()));
    timer_->start(this->stepFrequency());
}

QPointF Projectile::start()
{
    return this->start_;
}

void Projectile::setStart(QPointF start)
{
    this->start_ = start;
}

QPointF Projectile::targetPoint()
{
    return this->targetPoint_;
}

void Projectile::setTargetPoint(QPointF target)
{
    this->targetPoint_ = target;
}

double Projectile::range()
{
    return range_;
}

void Projectile::setRange(double range)
{
    range_ = range;
}

double Projectile::distanceTravelled()
{
    return distanceMoved_;
}

int Projectile::stepFrequency()
{
    return this->stepFrequency_;
}

void Projectile::setStepFrequency(int f)
{
    this->stepFrequency_ = f;
}

void Projectile::addToNoDamageList(Entity *entity)
{
    this->noDamageList_.insert(entity);
}

bool Projectile::isInNoDamageList(Entity *entity)
{
    return this->noDamageList_.count(entity) > 0;
}

/// Returns a list of entities that the projectile is currently colliding with
/// (at its current position).
std::unordered_set<Entity *> Projectile::collidingEntities()
{
    std::unordered_set<Entity*> ents = map()->entities(this->pointPos());

    // make sure the projectile itself is not in this list
    ents.erase(this);

    return ents;
}

void Projectile::setStepSize(int size)
{
    this->stepSize_ = size;
}

int Projectile::stepSize()
{
    return this->stepSize_;
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
    // move one step closer to target
    double amountMoved = this->moveBehavior_->onMoveStep();
    distanceMoved_ += amountMoved;

    // check for collisions
    std::unordered_set<Entity*> cEntities = collidingEntities();
    if (cEntities.size() > 0){
        this->collisionBehavior_->onCollisionWith(cEntities);
    }

    // check if range reached
    if (distanceMoved_ > range_){
        timer_->disconnect();
        this->rangeReachedBehavior_->onRangeReached();
    }
}
