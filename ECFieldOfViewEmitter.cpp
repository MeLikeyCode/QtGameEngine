#include "ECFieldOfViewEmitter.h"
#include "Entity.h"
#include <cassert>
#include "Map.h"
#include <QObject>
#include <QTimer>
#include <QLineF>
#include <QPointF>
#include <QPolygonF>

ECFieldOfViewEmitter::ECFieldOfViewEmitter(Entity *entity):
    entity_(entity),
    fieldOfViewAngle_(45),
    fieldOfViewDistance_(500),
    fieldOfViewCheckFrequency_(50),
    timerCheckFov_(new QTimer(this))
{
    // make sure passed in entity isn't null
    assert(entity != nullptr);

    // connect timer to keep checking fov
    connect(timerCheckFov_,&QTimer::timeout,this,&ECFieldOfViewEmitter::checkFov_);
    timerCheckFov_->start(fieldOfViewCheckFrequency_);
}

ECFieldOfViewEmitter::~ECFieldOfViewEmitter()
{
    // make sure timer disconnects
    // - timer will disconnect (since its a child of this qobject)
}

/// Executed periodically for the entity controller to check the field of view
/// of the controlled entity. Will emit a signal for each entity that
void ECFieldOfViewEmitter::checkFov_()
{
    // approach:
    // - if the controlled entity is destroyed, stop timer
    // - if the controlled entity is not in a map at this time, do nothing
    // - emit entityEntersFOV signal for any entity that is currently in
    //   the fov but wasn't earlier
    // - emit entityLeavesFOV signal for any entities that were in the fov
    //   earlier, but not anymore

    // if the controlled entity is destroyed, stop
    if (entity_.isNull()){
        timerCheckFov_->disconnect();
        return;
    }

    // if the controlled entity is not in a map, do nothing
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    // emit entityEntersFOV if any entities just entered the fov
    std::unordered_set<Entity*> entitiesInView = entitiesInView_();
    for (Entity* entity:entitiesInView){
        // if were not in fov earlier, emit
        if (entitiesInViewLastTime_.count(entity) == 0){
            entitiesInViewLastTime_.insert(entity);
            emit entityEntersFOV(entity);
        }
    }

    // emit entityLeavesFOV if any entities just left the fov
    for (Entity* entity:entitiesInViewLastTime_){
        // if the entiy is no longer in view, remove from list and emit
        if (entitiesInView.count(entity) == 0){
            entitiesInViewLastTime_.erase(entity);
            emit entityLeavesFOV(entity);
        }
    }

}

/// Returns all the entities in the fov of the entity being controlled.
std::unordered_set<Entity *> ECFieldOfViewEmitter::entitiesInView_()
{
    // - create QPolygon triangel w/ distance and angle
    // - pass this triangle to map to get entities in their

    Map* entitysMap = entity_->map();
    assert(entitysMap != nullptr); // make sure entitys map is not null

    QPointF p1(entity_->mapToMap(QPointF(0,0)));
    QLineF adjacent(p1,QPointF(-5,-5));
    adjacent.setAngle(-1 * entity_->facingAngle());
    adjacent.setLength(fieldOfViewDistance_);
    QLineF topL(adjacent);
    topL.setAngle(topL.angle() + fieldOfViewAngle_/2);
    QPointF p2(topL.p2());
    QLineF bottomL(adjacent);
    bottomL.setAngle(bottomL.angle() - fieldOfViewAngle_/2);
    QPointF p3(bottomL.p2());

    QVector<QPointF> points;
    points.append(p1);
    points.append(p2);
    points.append(p3);

    QPolygonF poly(points);

    std::unordered_set<Entity*> entities = entitysMap->entities(poly);
    entities.erase(entity_);

    return entities;
}
