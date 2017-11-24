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
    EntityController(entity),
    fieldOfViewAngle_(90),
    fieldOfViewDistance_(600),
    fieldOfViewCheckFrequency_(50),
    timerCheckFov_(new QTimer(this))
{
    // connect timer to keep checking fov
    connect(timerCheckFov_,&QTimer::timeout,this,&ECFieldOfViewEmitter::checkFov_);
    timerCheckFov_->start(fieldOfViewCheckFrequency_);

    // TODO: test remove
    polyItem_= new QGraphicsPolygonItem();
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    polyItem_->setBrush(brush);
    polyItem_->setOpacity(0.3);
    entityControlled()->map()->scene()->addItem(polyItem_);
}

/// Executed periodically for the entity controller to check the field of view
/// of the controlled entity. Will emit a signal for each entity that
/// enters/leaves its field of view.
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
    if (entityControlled() == nullptr){
        timerCheckFov_->disconnect();
        return;
    }

    // if the controlled entity is not in a map, do nothing
    Map* entitysMap = entityControlled()->map();
    if (entitysMap == nullptr)
        return;

    // emit entityEntersFOV if any entities just entered the fov
    std::unordered_set<Entity*> entsInView = entitiesInView();
    for (Entity* entity:entsInView){
        // if were not in fov earlier, emit
        if (entitiesInViewLastTime_.count(entity) == 0){
            entitiesInViewLastTime_.insert(entity);
            emit entityEntersFOV(entity);
        }
    }

    // emit entityLeavesFOV if any entities just left the fov
    std::unordered_set<Entity*> copy = entitiesInViewLastTime_;
    for (Entity* entity:copy){
        // if the entiy is no longer in view, remove from list and emit
        if (entsInView.count(entity) == 0){
            entitiesInViewLastTime_.erase(entity);
            emit entityLeavesFOV(entity);
        }
    }

}

/// Returns all the entities in the fov of the entity being controlled.
std::unordered_set<Entity *> ECFieldOfViewEmitter::entitiesInView()
{
    // - create QPolygon triangel w/ distance and angle
    // - pass this triangle to map to get entities in their

    Map* entitysMap = entityControlled()->map();
    assert(entitysMap != nullptr); // make sure entitys map is not null

    QPointF p1(entityControlled()->pos());
    QLineF adjacent(p1,QPointF(-5,-5));
    adjacent.setAngle(-1 * entityControlled()->facingAngle());
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

    // TEST remove TODO remove
    polyItem_->setPolygon(poly);

    std::unordered_set<Entity*> entities = entitysMap->entities(poly);
    entities.erase(entityControlled());

    return entities;
}
