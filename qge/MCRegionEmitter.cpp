#include "MCRegionEmitter.h"

#include "Map.h"
#include "Entity.h"
#include "STLWrappers.h"

qge::MCRegionEmitter::MCRegionEmitter(Map *controlledMap, const QPolygonF& region):
    MapController (controlledMap),
    region_(region)
{
    onEntityEnteredCB_ = [](MCRegionEmitter*,Entity*){/*do nothing*/};
    onEntityLeftCB_ = [](MCRegionEmitter*,Entity*){/*do nothing*/};

    connect(controlledMap,&Map::entityMoved,this,&MCRegionEmitter::onEntityMoved_);
    connect(controlledMap,&Map::entityAdded,this,&MCRegionEmitter::onEntityAdded_);
    connect(controlledMap,&Map::entityRemoved,this,&MCRegionEmitter::onEntityAdded_);
}

qge::MCRegionEmitter::~MCRegionEmitter()
{
    // disconnect all signals connected to this object's slots
    disconnect(controlledMap(),0,this,0);
}

/// Set a function to be called when an entity enters the region.
/// This is a convenience function, alternative to using the entityEntered() signal.
void qge::MCRegionEmitter::setOnEntityEnteredCB(const std::function<void (qge::MCRegionEmitter *, qge::Entity *)> &callback)
{
    onEntityEnteredCB_ = callback;
}

/// @see onEntityEnteredCB().
void qge::MCRegionEmitter::setOnEntityLeftCB(const std::function<void (qge::MCRegionEmitter *, qge::Entity *)> &callback)
{
    onEntityLeftCB_ = callback;
}

void qge::MCRegionEmitter::onEntityMoved_(qge::Map *sender, qge::Entity *entityMoved)
{
    // Executed when an Entity is moved in the Map being controlled.
    // Will see if the Entity *just* moved into our region; if so emit entityEntered() signal.
    // Will also see if Entity *just* moved out of our region; if so emit entityLeft() signal.

    // case: Entity *just* entered region
    if (!STLWrappers::contains(enteredEntities_,entityMoved)){ // ensure Entity isn't already in region
        QPolygonF boundingPolygon = entityMoved->boundingPolygonInMap();
        if (boundingPolygon.intersects(region_)){
            STLWrappers::add(enteredEntities_,entityMoved);
            onEntityEnteredCB_(this,entityMoved);
            emit entityEntered(this,entityMoved);
            return;
        }
    }

    // case: Entity *just* left region
    if (STLWrappers::contains(enteredEntities_,entityMoved)){
        QPolygonF boundingPolygon = entityMoved->boundingPolygonInMap();
        if (!boundingPolygon.intersects(region_)){
            STLWrappers::remove(enteredEntities_,entityMoved);
            onEntityLeftCB_(this,entityMoved);
            emit entityLeft(this,entityMoved);
            return;
        }
    }
}

void qge::MCRegionEmitter::onEntityAdded_(qge::Map *sender, qge::Entity *entityAdded)
{
    // Executed when an Entity is added to the Map being controlled.
    // Will see if the Entity was added in our region; if so emit signal.

    QPolygonF boundingPolygon = entityAdded->boundingPolygonInMap();
    if (boundingPolygon.intersects(region_)){
        STLWrappers::add(enteredEntities_,entityAdded);
        onEntityEnteredCB_(this,entityAdded);
        emit entityEntered(this,entityAdded);
    }
}

void qge::MCRegionEmitter::onEntityRemoved_(qge::Map *sender, qge::Entity *entityRemoved)
{
    // Executed when an Entity is removed from the Map being controlled.
    // If this Entity was in our region of interest, will emit entityLeft() signal (because it just left the map, thus the region)

    if (STLWrappers::contains(enteredEntities_,entityRemoved)){
        STLWrappers::remove(enteredEntities_,entityRemoved);
        onEntityLeftCB_(this,entityRemoved);
        emit entityLeft(this,entityRemoved);
    }
}
