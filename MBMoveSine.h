#ifndef MBMOVESINE_H
#define MBMOVESINE_H

#include <QObject>
#include <QPointer>
#include "Entity.h"
#include <QLineF>
#include "MoveBehavior.h"

class QTimer;

/// A MoveBehavior that moves the Entity at a sine looking pattern.
/// By default the entity will face its target pos before moving, but you can change this
/// by using setFaceTarget().
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// MBMoveSine* mb = new MBMoveSine(entity);
/// mb->setAmplitude(30);
/// mb->setWavelength(60);
/// mb->moveTo(somePos);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class MBMoveSine : public QObject, MoveBehavior
{
    Q_OBJECT
public:
    MBMoveSine(Entity* entity);

    void moveTo(const QPointF& pos);

    void setFaceTarget(bool tf);
    bool faceTarget();

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

public slots:
    void onMoveStep_();

protected:
    virtual void onStopMoving_();

private:
    QTimer* moveTimer_;

    double amplitude_;
    double wavelength_;
    QPointF targetPos_;
    QPointF startPos_;

    int stepSize_;
    int speed_;

    double range_;
    bool faceTarget_;

    double distanceMoved_;
};

#endif // MBMOVESINE_H
