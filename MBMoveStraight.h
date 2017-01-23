#ifndef MBMOVESTRAIGHT_H
#define MBMOVESTRAIGHT_H

#include <QPointer>
#include <QObject>
#include "Entity.h"
#include <QPointF>
#include "MoveBehavior.h"

class QTimer;

/// A MoveBehavior that moves the Entity in a straight line.
/// By default, the entity will move at a speed determined by its Entity::speed() function,
/// but you can force it to move at a specified speed by calling setSpeed();
/// By default the entity will not face the target position, but you can toggle this
/// by calling setFaceTarget();
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// MBMoveStraight* mb = new MBMoveStraight(entity);
/// mb->setFaceTarget(true); // make entity face target position before moving
/// mb->setStepSize(5); // sets the size of each movement step (lower = smoother movement but more less effecient)
/// mb->moveTo(somePoint); // tell entity to start moving towards some point
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class MBMoveStraight: public QObject, public MoveBehavior
{
    Q_OBJECT
public:
    MBMoveStraight(Entity* entity);

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

    void setFaceTarget(bool tf);
    bool faceTarget();

    virtual void moveTo(const QPointF& pos);

public slots:
    void moveStep_();

protected:
    virtual void onStopMoving_();

private:
    int speed_;
    bool faceTarget_;

    QTimer* moveTimer_;

    // more internal
    double initialAngle_;
    QPointF targetPos_;
    int stepSize_;
};

#endif // MBMOVESTRAIGHT_H
