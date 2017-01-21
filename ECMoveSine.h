#ifndef ECMOVESINE_H
#define ECMOVESINE_H

#include <QObject>
#include <QPointer>
#include "Entity.h"
#include <QLineF>

class QTimer;

/// An entity controller that makes the controlled entity move at a sine looking pattern.
/// Just try it out and see what it looks like.
///
/// Example usage:
/// ECMoveSine* c = new ECMoveSine(entity);
/// c->setAmplitude(30);
/// c->setWavelength(60);
/// c->moveTowards(somePos);
/// c->stopMoving();
/// c->isMoving(); // find out if the controlled entity is currently moving or not
class ECMoveSine : public QObject
{
    Q_OBJECT
public:
    ECMoveSine(Entity& entity);

    void moveTowards(QPointF pos);
    void stopMoving();

public slots:
    void onMoveStep_();

private:
    QPointer<Entity> entity_;
    QTimer* moveTimer_;

    double amplitude_;
    double wavelength_;
    QPointF targetPos_;
    QPointF startPos_;

    int stepSize_;
    int speed_;

    double range_;
    bool faceTarget_;
    bool isMoving_;

    double distanceMoved_;
};

#endif // ECMOVESINE_H
