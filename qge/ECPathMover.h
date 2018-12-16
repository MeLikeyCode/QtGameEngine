#pragma once

#include "Vendor.h"

#include "Entity.h"
#include "ECMover.h"

class QTimer;

namespace qge{

class AsyncShortestPathFinder;
class ECRotater;

/// A Mover that moves the Entity in a path finding way. The entity will move
/// using a path finding algorithm. You can specifiy if the entity should
/// continue to face its target position or if it should face the direction its
/// heading into.
///
/// Example usage:
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// PathMover* pm = new PathMover(entity);
/// pm->setAlwaysFaceTargetPosition();
/// pm->moveEntity(toSomePoint);
/// connect(pm,&PathMover::moved,this,myCallback);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @author Abdullah Aghazadah
/// @date 11/28/16
class ECPathMover: public ECMover
{
    Q_OBJECT
public:
    ECPathMover(Entity* entity = nullptr);

    void setSpeed(int speed);
    int speed();

    void setStepSize(double stepSize);
    double stepSize();

    bool alwaysFaceTargetPosition();
    void setAlwaysFaceTargetPosition(bool tf);

public slots:
    void onPathCalculated_(std::vector<QPointF> path);
    void onMoveStep_();
signals:
    /// Emitted each time the controlled entity moves.
    void moved(const QPointF& toPos);

protected:
    virtual void moveEntity_(const QPointF &toPos);
    virtual void stopMovingEntity_();

private:
    // options
    bool alwaysFaceTargetPosition_; // controlled entity should continueously face last pos in
                                    // path (instead of facing forward in each segment of path)
    int stepSize_;  // how "granular" the movement should be

    QTimer* moveTimer_;
    std::unique_ptr<AsyncShortestPathFinder> pf_;
    ECRotater* rotater_;

    // moving helper attributes
    std::vector<QPointF> pointsToFollow_;
    int targetPointIndex_;
    bool currentlyMoving_;

    // moving helper functions
    bool targetPointReached_();
    void stepTowardsTarget_();
};

}
