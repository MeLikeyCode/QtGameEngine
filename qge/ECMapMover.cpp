#include "ECMapMover.h"

#include "Map.h"
#include "Game.h"
#include "MapGrid.h"
#include "Node.h"

using namespace qge;

ECMapMover::ECMapMover(Entity *entity):
    EntityController(entity),
    borderThreshold_(10)
{
    assert(entity != nullptr);

    // listen to when entity moves
    connect(entity,&Entity::moved,this,&ECMapMover::onEntityMoved);
}

/// Sets the border threshold. Whenever the controlled entity gets within
/// this distance to any of the borders of its map, it will be transported
/// to the next map in that direction.
void ECMapMover::setBorderThreshold(double threshold)
{
    borderThreshold_ = threshold;
}

/// Returns the border threshold.
/// See setBorderThreshold() for more info.
double ECMapMover::borderThreshold()
{
    return borderThreshold_;
}

/// Executed when the controlled entity moves.
/// Will see if it should move to new border map.
void ECMapMover::onEntityMoved(Entity *controlledEntity, QPointF fromPos, QPointF toPos)
{
    // do nothing if controlled entity is not in a map
    Entity* entity = entityControlled();
    Map* entitysMap = entity->map();
    if (entitysMap == nullptr)
        return;

    // do nothing if entitys map is not in a map grid
    Game* entitysGame = entitysMap->game();
    if (entitysGame == nullptr)
        return;

    MapGrid* mapGrid = entitysGame->mapGrid();
    if (mapGrid == nullptr)
        return;
    if (!mapGrid->contains(entitysMap))
        return;

    // other wise, move entity to next maps

    // set up variables
    Node mapPos = mapGrid->positionOf(entitysMap);
    QPointF entityPos = entity->pos();

    // if entity moved high up enough, move to top map
    if (entityPos.y() < borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x(),mapPos.y()-1);
        if (m){
            double fracAlong = entity->x() / entitysMap->width();
            m->addEntity(entity);
            entity->setPos(QPointF(fracAlong * m->width(),m->height() - borderThreshold_*2));
        }
    }

    // if entity moved low enough, move to bot map
    if (entityPos.y() > entitysMap->height() - borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x(),mapPos.y()+1);
        if (m){
            double fracAlong = entity->x() / entitysMap->width();
            m->addEntity(entity);
            entity->setPos(QPointF(fracAlong * m->width(),borderThreshold_*2));
        }
    }

    // if entity moved left enough, move to left map
    if (entityPos.x() < borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x()-1,mapPos.y());
        if (m){
            double fracAlong = entity->y() / entitysMap->height();
            m->addEntity(entity);
            entity->setPos(QPointF(m->width() - borderThreshold_*2,fracAlong * m->height()));
        }
    }

    // if entity moved right enough, move to right map
    if (entityPos.x() > entitysMap->width() - borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x()+1,mapPos.y());
        if (m){
            double fracAlong = entity->y() / entitysMap->height();
            m->addEntity(entity);
            entity->setPos(QPointF(borderThreshold_*2,fracAlong * m->height()));
        }
    }
}
