#include "ECMoveToNextMap.h"
#include <cassert>
#include "Map.h"
#include "Game.h"
#include "MapGrid.h"
#include "Node.h"

ECMoveToNextMap::ECMoveToNextMap(Entity *entity):
    entity_(entity),
    borderThreshold_(100)
{
    assert(entity != nullptr);

    // listen to when entity moves
    connect(entity,&Entity::moved,this,&ECMoveToNextMap::onEntityMoved);
}

/// Sets the border threshold. Whenever the controlled entity gets within
/// this distance to any of the borders of its map, it will be transported
/// to the next map in that direction.
void ECMoveToNextMap::setBorderThreshold(double threshold)
{
    borderThreshold_ = threshold;
}

/// Returns the border threshold.
/// See setBorderThreshold() for more info.
double ECMoveToNextMap::borderThreshold()
{
    return borderThreshold_;
}

/// Executed when the controlled entity moves.
/// Will see if it should move to new border map.
void ECMoveToNextMap::onEntityMoved(Entity *controlledEntity, QPointF fromPos, QPointF toPos)
{
    // do nothing if controlled entity is not in a map
    Map* entitysMap = entity_->map();
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
    QPointF entityPos = entity_->pos();

    // if entity moved high up enough, move to top map
    if (entityPos.y() < borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x(),mapPos.y()-1);
        if (m){
            m->addEntity(entity_);
            entity_->setPos(QPointF(entity_->x(),m->height() - borderThreshold_*2));
        }
    }

    // if entity moved low enough, move to bot map
    if (entityPos.y() > entitysMap->height() - borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x(),mapPos.y()+1);
        if (m){
            m->addEntity(entity_);
            entity_->setPos(QPointF(entity_->x(),borderThreshold_*2));
        }
    }

    // if entity moved left enough, move to left map
    if (entityPos.x() < borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x()-1,mapPos.y());
        if (m){
            m->addEntity(entity_);
            entity_->setPos(QPointF(m->width() - borderThreshold_*2,entity_->y()));
        }
    }

    // if entity moved right enough, move to right map
    if (entityPos.x() > entitysMap->width() - borderThreshold_){
        Map* m = mapGrid->mapAt(mapPos.x()+1,mapPos.y());
        if (m){
            m->addEntity(entity_);
            entity_->setPos(QPointF(borderThreshold_*2,entity_->y()));
        }
    }
}
