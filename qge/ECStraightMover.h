#pragma once

#include "Vendor.h"

#include "Entity.h"
#include "ECMover.h"

class QTimer;

namespace qge{

/// A Mover that moves the Entity in a straight line. By default, the entity
/// will be moved at a speed determined by its Entity::speed() function, but
/// you can force it to be moved at a specified speed by calling setSpeed(); By
/// default the entity will face the target position prior to moving, but you
/// can alter this behavior by using setFaceTarget(bool).
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// StraightMover* sm = new StraightMover(entity);
/// sm->setStepSize(5); // sets the size of each movement step ("movement granularity")
/// sm->moveEntity(toSomePoint); // tell entity to start moving towards some point
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Abdullah Aghazadah
class ECStraightMover: public ECMover
{
    Q_OBJECT
public:
    ECStraightMover(Entity* entity = nullptr);

    void setSpeed(int speed);
    int speed();

    void setStepSize(int stepSize);
    int stepSize();

    void setFaceTarget(bool tf);
    bool faceTarget();

public slots:
    void onMoveStep_();

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

}
