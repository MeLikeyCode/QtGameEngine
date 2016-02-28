#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include <QObject>
#include <QPointF>
#include <unordered_set>
#include <vector>
#include <QTimer>

/// Abstract class that represents a projectile that moves to a target.
/// A projectile has a point that it should be traveling towards.
/// When it reaches that point, it will call the virtual targetReached()
/// function. Along the way, if it collides with any Entities, it will
/// call the virtual collidedWith() function.
///
/// In order to create your own projectile, sub class this and then implement
/// the following virtual functions:
/// -moveStep();
/// -collidedWith(std::vector<Entity*> entities);
/// -targetReached()
///
/// The moveStep function will be called every time the projectile needs to
/// move. You can use setStepFrequency() to set how often this function is called.
///
/// The collidedWith function will be called whenever the projectile has collided
/// with some entity(s).
///
/// The targetReached() function will be called whenever the projectile has reached
/// its aimed target.
///
/// During the life of a projectile, every so often the moveStep will be called,
/// which should move the projectile 1 step forward in its path, after it has moved
/// forward, the projectile will be tested for collision and every entity that
/// it collides with will be returned via the collidedWith function. Finally,
/// when a projectile has reached its endpoint, it will call the targetReached
/// function. Most of the times, you'd want to destroy the projectile when it
/// reaches its target.
///
/// Note that several subclasses of Projectile exist that provide
/// some default implementations for some of these virtual functions.
/// StraightProjectile is one of these subclasses.
///
/// Every projectile maintains a list of entities that it should not damage.
/// The Entity that spawns a projectile should be adde to this list to prevent
/// the projectile from damaging him (addToNoDamageList()).
///
/// @author Abdullah Aghazadah
/// @date 2/21/16
class Projectile : public QObject, public Entity
{
    Q_OBJECT
public:
    Projectile();

    void start();

    QPointF targetPoint();
    void setTargetPoint(QPointF target);

    int stepFrequency();
    void setStepFrequency(int f);

    void setStepSize(int size);
    int stepSize();

    void addToNoDamageList(Entity* entity);
    bool isInNoDamageList(Entity* entity);

    std::vector<Entity*> collidingEntities();

    virtual void targetReached() = 0;
    // this function is called whenever the projectile collides with any
    // entities along its path
    virtual void collidedWith(std::vector<Entity*> entities) = 0;
    virtual void moveStep() = 0;

public slots:
    void step_();

private:
    QPointF targetPoint_;
    std::unordered_set<Entity*> noDamageList_;
    int stepFrequency_;
    int stepSize_;
    QTimer* timer_;


};

#endif // PROJECTILE_H
