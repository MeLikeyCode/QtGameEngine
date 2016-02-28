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
    std::vector<Entity*> ents = map()->entities(this->pointPos());
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
/// First, a virtual function is called that determines where the projectile
/// move.
/// Second, all colliding entities are detected at the new position, and
/// passed into a second virtual function that determines what to do with em.
/// Third, if the targetPoint is reached, will call a virtual function,
/// and disconnect timer.
void Projectile::step_()
{
    // move one step closer to target
    this->moveStep();

    // check for collisions
    std::vector<Entity*> cEntities = collidingEntities();
    if (cEntities.size() > 0){
        this->collidedWith(cEntities);
    }

    // check if done moving
    int EPSILON = stepSize();
    double distance = QLineF(pointPos(),targetPoint()).length();
    if (distance < EPSILON){
        timer_->disconnect();
        this->targetReached();
    }
}
