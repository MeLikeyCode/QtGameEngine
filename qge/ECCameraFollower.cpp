#include "ECCameraFollower.h"

#include "Map.h"
#include "Game.h"

using namespace qge;

ECCameraFollower::ECCameraFollower(Entity *entity):
    EntityController(entity)
{
    // make sure passed in entity is not nullptr
    assert(entity != nullptr);

    // listen to when the entity moves
    connect(entity,&Entity::moved,this,&ECCameraFollower::onEntityMoved_);
}

/// Executed whenever the controlled entity moves.
/// Will make game follow it.
void ECCameraFollower::onEntityMoved_(Entity *entity, QPointF fromPos, QPointF toPos)
{
    // if the entity is not in a map at this moment, do nothing
    Map* entitysMap = entity->map();
    if (entitysMap == nullptr)
        return;

    // if the entitys map is not in a game right now, do nothing
    Game* game = entitysMap->game();
    if (game == nullptr)
        return;

    // other wise, make game's cam move to entity's location
    game->setCenterCamPos(toPos);
}
