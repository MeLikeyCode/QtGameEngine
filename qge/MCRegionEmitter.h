#ifndef MCREGIONENTEREDEMITTER_H
#define MCREGIONENTEREDEMITTER_H

#include "Vendor.h"

#include "MapController.h"

namespace qge{

class Entity;

/// A MapController that tells you (via a signal) when an Entity enters or leaves a certain region of a Map.
///
/// Example
/// -------
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// QPolygonF region; // assume properly initialized QPolygonF, this is the region of the Map you want to monitor
/// MCRegionEmitter* regionEmitter = new MCRegionEmitter(map, region);
///
/// // Now, whenever an Entity enters the specified region of the Map, regionEmitter will emit the entityEntered signal.
/// // Similarly, whenever an Entity leaves the specified region, regionEmitter will emit the entityLeft signal.
///
/// // An alternative to the signals is to use setOnEntityEnteredCB() and setOnEntityLeftCB(), which allow you
/// // to set a function you want called when an entity enters/leaves the region.
///
/// regionEmitter->setOnEntityEnteredCB([](MCRegionEmitter* sender, Entity* entity){
///     // do something with 'entity' (the entity that just entered the region
/// });
///
/// regionEmitter->setOnEntityLeftCB([](MCRegionEmitter* sender, Entity* entity){
///     // do something with 'entity' (the entity that just left the region
/// });
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class MCRegionEmitter: public MapController
{
    Q_OBJECT
public:
    MCRegionEmitter(Map* controlledMap, const QPolygonF& region);
    virtual ~MCRegionEmitter();

    // alternatives to using the signals
    void setOnEntityEnteredCB(const std::function<void(MCRegionEmitter*,Entity*)>& callback);
    void setOnEntityLeftCB(const std::function<void(MCRegionEmitter*,Entity*)>& callback);

signals:
    void entityEntered(MCRegionEmitter* sender, Entity* enteringEntity);
    void entityLeft(MCRegionEmitter* sender, Entity* leavingEntity);

private:
    QPolygonF region_;
    std::set<Entity*> enteredEntities_;
    std::function<void(MCRegionEmitter*,Entity*)> onEntityEnteredCB_;
    std::function<void(MCRegionEmitter*,Entity*)> onEntityLeftCB_;

    void onEntityMoved_(Map* sender, Entity* entityMoved);
    void onEntityAdded_(Map* sender, Entity* entityAdded);
    void onEntityRemoved_(Map* sender, Entity* entityRemoved);
};

}

#endif // MCREGIONENTEREDEMITTER_H
