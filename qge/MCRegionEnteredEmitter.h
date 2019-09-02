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
/// QPolygonF region; // assume properly initialized QPolygonF
/// MCRegionEnteredEmitter* regionEmitter = new MCRegionEnteredEmitter(map, region);
/// // Now, whenever an Entity enters the specified region of the map, regionEmitter will emit the entityEntered signal.
/// // Similarly, whenever an Entity leaves the specified region, regionEmitter will emit the entityLeft signal.
/// // The controlled map owns the lifetime.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class MCRegionEnteredEmitter: public MapController
{
    Q_OBJECT
public:
    MCRegionEnteredEmitter(Map* controlledMap, const QPolygonF&);
    virtual ~MCRegionEnteredEmitter();

    // alternatives to using the signals
    void setOnEntityEnteredCB(const std::function<void(Map*,Entity*)>& callback);
    void setOnEntityLeftCB(const std::function<void(Map*,Entity*)>& callback);

signals:
    void entityEntered(MCRegionEnteredEmitter* sender, Entity* enteringEntity);
    void entityLeft(MCRegionEnteredEmitter* sender, Entity* leavingEntity);

private:
    QPolygonF region_;
    std::set<Entity*> enteredEntities_;
    std::function<void(Map*,Entity*)> onEntityEnteredCB_;
    std::function<void(Map*,Entity*)> onEntityLeftCB_;

    void onEntityMoved_(Map* sender, Entity* entityMoved);
    void onEntityAdded_(Map* sender, Entity* entityAdded);
    void onEntityRemoved_(Map* sender, Entity* entityRemoved);
};

}

#endif // MCREGIONENTEREDEMITTER_H
