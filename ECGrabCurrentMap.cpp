#include "ECGrabCurrentMap.h"
#include <cassert>
#include "Map.h"
#include "Game.h"

ECGrabCurrentMap::ECGrabCurrentMap(Entity *entity):
    EntityController(entity)
{
    assert(entity != nullptr);

    // listen to when entity enters maps
    connect(entity,&Entity::mapEntered,this,&ECGrabCurrentMap::onEntityEntersMap);
}

/// Executed whenever the controlled entity enters a map.
/// Will set that map as the game's current map.
void ECGrabCurrentMap::onEntityEntersMap(Entity *controlledEntity, Map *mapJustEntered, Map *oldMap)
{
    // do nothing if the entered map is not in a game
    Game* game = mapJustEntered->game();
    if (game == nullptr)
        return;

    game->setCurrentMap(mapJustEntered);
}
