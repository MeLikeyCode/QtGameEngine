#ifndef SINEMOVER_H
#define SINEMOVER_H

#include <QObject>
#include <QPointer>
#include "Entity.h"
#include <QLineF>
#include "ECMover.h"

class QTimer;

/// A Mover that moves the Entity at a sine looking pattern. By default the
/// entity will face its target pos before moving, but you can change this by
/// using setFaceTarget().
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// SineMover* sm = new SineMover(entity);
/// sm->setAmplitude(30);
/// sm->setWavelength(60);
/// sm->setSpeed(70);
/// sm->moveEntity(toSomePoint);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Abdullah Aghazadah
class ECSineMover : ECMover
{
    Q_OBJECT
public:
    ECSineMover(Entity* entity=nullptr);

    void setFaceTarget(bool tf);
    bool faceTarget();

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

public slots:
    void onMoveStep_();

protected:
    virtual void moveEntity_(const QPointF& pos);
    virtual void stopMovingEntity_();

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

#endif // SINEMOVER_H
