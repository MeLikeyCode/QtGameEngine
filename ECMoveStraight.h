#ifndef ECMOVESTRAIGHT_H
#define ECMOVESTRAIGHT_H

#include <QPointer>
#include <QObject>
#include "Entity.h"
#include <QPointF>

class QTimer;

/// An entity controller that makes the controlled entity move straight towards
/// some point. By default, the entity will move at a speed determined by its Entity::speed() function,
/// but you can force it to move at a specified speed by calling setSpeed();
/// By default the controlled entity will not face the target position, but you can toggle this
/// by calling setFaceTarget();
///
/// Example usage:
/// ECMoveStraight* c = new ECMoveStraight(entity);
/// c->moveTowards(QPointF(50,200)); // tell entity to start moving towards some point
/// c->stopMoving();
/// c->isMoving(); // find it weather the controlled entity is currently being moved
class ECMoveStraight: public QObject
{
public:
    ECMoveStraight(Entity& entity);

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

    void moveTowards(QPointF pos);
    void stopMoving();

    void setFaceTarget(bool tf);
    bool faceTarget();

    bool isMoving();

public slots:
    void moveStep_();

private:
    int speed_;
    bool faceTarget_;
    bool isMoving_;

    QPointer<Entity> entity_;
    QTimer* moveTimer_;

    // more internal
    double initialAngle_;
    QPointF targetPos_;
    int stepSize_;
};

#endif // ECMOVESTRAIGHT_H
