#include "ECCurrentMapGrabber.h"

#include "Map.h"
#include "Game.h"

using namespace qge;

ECCurrentMapGrabber::ECCurrentMapGrabber(Entity *entity):
    EntityController(entity)
{
    assert(entity != nullptr);

    // listen to when entity enters maps
    connect(entity,&Entity::mapEntered,this,&ECCurrentMapGrabber::onEntityEntersMap);
}

/// Executed whenever the controlled entity enters a map.
/// Will set that map as the game's current map.
void ECCurrentMapGrabber::onEntityEntersMap(Entity *controlledEntity, Map *mapJustEntered, Map *oldMap)
{
    // do nothing if entity left a map
    if (mapJustEntered == nullptr)
        return;

    Game::game->setCurrentMap(mapJustEntered);
}
