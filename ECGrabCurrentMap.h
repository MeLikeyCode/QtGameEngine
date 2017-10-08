#ifndef ECGRABCURRENTMAP_H
#define ECGRABCURRENTMAP_H

#include <QPointer>

#include "EntityController.h"
#include "Entity.h"

class Map;

/// An entity controller (TODO link to doc) that will set the game's current
/// map to always be the map that the controlled entity is in.
///
/// Example usage:
/// ECGrabCurrentMap* c = new ECGrabCurrentMap(entity);
///
/// That is all. From then on, whenever the controlled entity moves to a different
/// map, that map will be set as the game's current map. Remember that the game's
/// current map is basically the map that is being visualized.
/// @author Abdullah Aghazadah
/// @date 12/7/16
class ECGrabCurrentMap: public EntityController
{
    Q_OBJECT
public:
    ECGrabCurrentMap(Entity* entity);
public slots:
    void onEntityEntersMap(Entity* controlledEntity, Map* mapJustEntered, Map* oldMap);
};

#endif // ECGRABCURRENTMAP_H
