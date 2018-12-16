#include "ECFieldOfViewEmitter.h"

#include "Entity.h"
#include "Map.h"

using namespace qge;

ECFieldOfViewEmitter::ECFieldOfViewEmitter(Entity *entity, double fovAngle, double fovDistance):
    EntityController(entity),
    fieldOfViewAngle_(fovAngle),
    fieldOfViewDistance_(fovDistance),
    fieldOfViewCheckDelayMs_(50),
    timerCheckFov_(new QTimer(this)),
    showFOV_(false)
{
    // connect timer to keep checking fov
    connect(timerCheckFov_,&QTimer::timeout,this,&ECFieldOfViewEmitter::checkFov_);
    timerCheckFov_->start(fieldOfViewCheckDelayMs_);

    // create polygon item for visualization purposes
    fovVisual_= new QGraphicsPolygonItem();
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    fovVisual_->setBrush(brush);
    fovVisual_->setOpacity(0.3);
}

ECFieldOfViewEmitter::~ECFieldOfViewEmitter()
{
    // clean up field of view visualization
    ensureFOVVisualIsRemoved_();
    delete fovVisual_;
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
            //qDebug() << "entity entered fov";
            emit entityEntersFOV(entity);
        }
    }

    // emit entityLeavesFOV if any entities just left the fov
    std::unordered_set<Entity*> copy = entitiesInViewLastTime_;
    for (Entity* entity:copy){
        // if the entiy is no longer in view, remove from list and emit
        if (entsInView.count(entity) == 0){
            entitiesInViewLastTime_.erase(entity);
            // qDebug() << "entity left fov";
            emit entityLeavesFOV(entity);
        }
    }

}

void ECFieldOfViewEmitter::ensureFOVVisualIsRemoved_()
{
    auto entity = entityControlled();
    if (entity){
        auto map = entity->map();
        if (map)
            map->scene()->removeItem(fovVisual_);
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

    fovVisual_->setPolygon(poly);

    if (showFOV_){
        entitysMap->scene()->removeItem(fovVisual_);
        entitysMap->scene()->addItem(fovVisual_);
    }

    std::unordered_set<Entity*> entities = entitysMap->entities(poly);
    entities.erase(entityControlled());

    return entities;
}

/// Sets how often the controller should check the field of view in order to determine
/// if something has entered or left it.
void ECFieldOfViewEmitter::setCheckFrequency(double timesPerSecond)
{
    timerCheckFov_->stop();
    double timesPerMS = timesPerSecond/1000.0;
    fieldOfViewCheckDelayMs_ = 1/timesPerMS;
    timerCheckFov_->start(fieldOfViewCheckDelayMs_);
}

/// See setCheckFrequency().
double ECFieldOfViewEmitter::checkFrequency() const
{
    double timesPerMS = 1 / fieldOfViewCheckDelayMs_;
    double timesPerS = timesPerMS * 1000;
    return timesPerS;
}

/// Turns the controller on.
/// When on, signals will be emitted.
/// When off, signals will not be emitted.
/// Turning it on when its already on or turning it off when its already off does nothing.
void ECFieldOfViewEmitter::turnOn()
{
    timerCheckFov_->start(fieldOfViewCheckDelayMs_);
}

/// See turnOn().
void ECFieldOfViewEmitter::turnOff()
{
    timerCheckFov_->stop();
}

/// If true is passed in, draws the field of view of the controlled entity.
/// If false is passed in, does not draw the field of view. Simple enough.
void ECFieldOfViewEmitter::setShowFOV(bool tf)
{
    showFOV_ = tf;

    // if false, make sure visual is removed
    if (!showFOV_){
        ensureFOVVisualIsRemoved_();
    }
}
