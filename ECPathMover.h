#ifndef ECPATHMOVER_H
#define ECPATHMOVER_H

#include <QPointer>
#include <QPointF>
#include <vector>
#include <QPointF>
#include <QObject>
#include "Entity.h"

class QTimer;
class AsyncShortestPathFinder;
class ECRotater;

/// An entity controller (TODO: add doc link) that can move an entity to a specified
/// pos on its map. The controlled entity will move using a path finding algorithm.
/// You can specifiy if the controlled entity should continue to face its target
/// position or if it should face the direction its heading into.
///
/// Example usage:
/// ECPathMover* pm = new ECPathMover(entity);
/// pm->setAlwaysFaceTargetPosition(); // make entity always face the position he's heading to
/// pm->moveEntityTo(pos);
/// pm->entityIsCurrentlyMoving(); // is the entity currently moving?
/// pm->stopMoving();
/// connect(pm,&ECPathMover::moved,this,myCallback);
///
/// @author Abdullah Aghazadah
/// @date 11/28/16
class ECPathMover: public QObject
{
    Q_OBJECT
public:
    ECPathMover(Entity* entity);

    void moveEntityTo(const QPointF& pos);
    void stopMoving();
    bool entityIsCurrentlyMoving();

    bool alwaysFaceTargetPosition();
    void setAlwaysFaceTargetPosition(bool tf);

public slots:
    void onPathCalculated_(std::vector<QPointF> path);
    void moveStep_();
signals:
    /// Emitted each time the controlled entity moves.
    void moved(const QPointF& toPos);

private:
    // options
    bool alwaysFaceTargetPosition_;

    QPointer<Entity> entity_;

    QTimer* moveTimer_;
    AsyncShortestPathFinder* pf_;
    ECRotater* rotater_;

    // moving parameters
    int stepSize_;
    int stepFrequency_;

    // moving helper attributes
    std::vector<QPointF> pointsToFollow_;
    int targetPointIndex_;
    bool currentlyMoving_;

    // moving helper functions
    bool targetPointReached_();
    void stepTowardsTarget_();
};

#endif // ECPATHMOVER_H
