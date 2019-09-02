#include "MCRegionEnteredEmitter.h"

#include "Map.h"
#include "Entity.h"
#include "STLWrappers.h"

qge::MCRegionEnteredEmitter::MCRegionEnteredEmitter(Map *controlledMap, const QPolygonF& region):
    MapController (controlledMap),
    region_(region)
{
    onEntityEnteredCB_ = [](Map*,Entity*){/*do nothing*/};
    onEntityLeftCB_ = [](Map*,Entity*){/*do nothing*/};

    connect(controlledMap,&Map::entityMoved,this,&MCRegionEnteredEmitter::onEntityMoved_);
    connect(controlledMap,&Map::entityAdded,this,&MCRegionEnteredEmitter::onEntityAdded_);
    connect(controlledMap,&Map::entityRemoved,this,&MCRegionEnteredEmitter::onEntityAdded_);
}

qge::MCRegionEnteredEmitter::~MCRegionEnteredEmitter()
{
    // disconnect all signals connected to this object's slots
    disconnect(controlledMap(),0,this,0);
}

/// Set a function to be called when an entity enters the region.
/// This is a convenience function, alternative to using the entityEntered() signal.
void qge::MCRegionEnteredEmitter::setOnEntityEnteredCB(const std::function<void (qge::Map *, qge::Entity *)> &callback)
{
    onEntityEnteredCB_ = callback;
}

/// @see onEntityEnteredCB().
void qge::MCRegionEnteredEmitter::setOnEntityLeftCB(const std::function<void (qge::Map *, qge::Entity *)> &callback)
{
    onEntityLeftCB_ = callback;
}

void qge::MCRegionEnteredEmitter::onEntityMoved_(qge::Map *sender, qge::Entity *entityMoved)
{
    // Executed when an Entity is moved in the Map being controlled.
    // Will see if the Entity *just* moved into our region; if so emit entityEntered() signal.
    // Will also see if Entity *just* moved out of our region; if so emit entityLeft() signal.

    // case: Entity *just* entered region
    if (!STLWrappers::contains(enteredEntities_,entityMoved)){ // ensure Entity isn't already in region
        QPolygonF boundingPolygon = entityMoved->boundingPolygonInMap();
        if (boundingPolygon.intersects(region_)){
            STLWrappers::add(enteredEntities_,entityMoved);
            emit entityEntered(this,entityMoved);
            return;
        }
    }

    // case: Entity *just* left region
    if (STLWrappers::contains(enteredEntities_,entityMoved)){
        QPolygonF boundingPolygon = entityMoved->boundingPolygonInMap();
        if (!boundingPolygon.intersects(region_)){
            STLWrappers::remove(enteredEntities_,entityMoved);
            emit entityLeft(this,entityMoved);
            return;
        }
    }
}

void qge::MCRegionEnteredEmitter::onEntityAdded_(qge::Map *sender, qge::Entity *entityAdded)
{
    // Executed when an Entity is added to the Map being controlled.
    // Will see if the Entity was added in our region; if so emit signal.

    QPolygonF boundingPolygon = entityAdded->boundingPolygonInMap();
    if (boundingPolygon.intersects(region_)){
        STLWrappers::add(enteredEntities_,entityAdded);
        emit entityEntered(this,entityAdded);
    }
}

void qge::MCRegionEnteredEmitter::onEntityRemoved_(qge::Map *sender, qge::Entity *entityRemoved)
{
    // Executed when an Entity is removed from the Map being controlled.
    // If this Entity was in our region of interest, will emit entityLeft() signal (because it just left the map, thus the region)

    if (STLWrappers::contains(enteredEntities_,entityRemoved)){
        STLWrappers::remove(enteredEntities_,entityRemoved);
        emit entityLeft(this,entityRemoved);
    }
}
