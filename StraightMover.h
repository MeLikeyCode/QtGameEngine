#ifndef STRAIGHTMOVER_H
#define STRAIGHTMOVER_H

#include <QPointer>
#include <QObject>
#include "Entity.h"
#include <QPointF>
#include "Mover.h"

class QTimer;

/// A Mover that moves the Entity in a straight line. By default, the entity
/// will move at a speed determined by its Entity::speed() function, but you
/// can force it to move at a specified speed by calling setSpeed(); By default
/// the entity will face the target position prior to moving, but you can set
/// alter this behavior by using setFaceTarget(bool yesNo).
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// StraightMover* sm = new StraightMover(entity);
/// sm->setFaceTarget(true); // make entity face target position before moving
/// sm->setStepSize(5); // sets the size of each movement step
/// sm->moveEntity(toSomePoint); // tell entity to start moving towards some point
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class StraightMover: public QObject, public Mover
{
    Q_OBJECT
public:
    StraightMover(Entity* entity = nullptr);

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

    void setFaceTarget(bool tf);
    bool faceTarget();

public slots:
    void moveStep_();

protected:
    virtual void moveEntity_(const QPointF& pos);
    virtual void stopMovingEntity_();

private:
    int speed_;
    bool faceTarget_;

    QTimer* moveTimer_;

    // more internal
    double initialAngle_;
    QPointF targetPos_;
    int stepSize_;
};

#endif // STRAIGHTMOVER_H
