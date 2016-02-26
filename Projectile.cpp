#include "Projectile.h"
#include "Map.h"

Projectile::Projectile()
{

}

/// Start moving the projectile.
void Projectile::start()
{
    // connect timer
    timer_ = new QTimer();
    connect(timer_,SIGNAL(timeout()),this,SLOT(step_()));
    timer_->start(this->stepFrequency());
}

QPointF Projectile::targetPoint()
{
    return this->targetPoint_;
}

void Projectile::setTargetPoint(QPointF target)
{
    this->targetPoint_ = target;
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
std::vector<Entity *> Projectile::collidingEntities()
{
    std::vector<Entity*> ents = map()->entities(mapToMap(this->pointPos()));
    return ents;
}

/// Executed every "step" the projectile needs to take.
/// First, a virtual function is called that determines where the projectile
/// move.
/// Second, all colliding entities are detected at the new position, and
/// passed into a second virtual function that determines what to do with em.
/// Third, if the targetPoint is reached, will call a virtual function,
/// and disconnect timer.
void Projectile::step_()
{
    // first call the virtual move so the projectile can move
    // in whatever way it wants to
    this->moveStep();

    // then call virtual function passing in all collided items, so
    // subclass can decide what to do
    std::vector<Entity*> cEntities = collidingEntities();
    this->collidedWith(cEntities);

    // if target point is reached, call virtual function
//    this->targetReached();
//    timer_->disconnect();
}
