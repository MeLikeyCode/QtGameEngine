#include "CHealthShower.h"

#include "STLWrappers.h"
#include "Entity.h"
#include "Map.h"
#include "Bar.h"
#include "EntitySprite.h"

using namespace qge;

CHealthShower::CHealthShower():
    timer_(new QTimer(this))
{
    connect(timer_,&QTimer::timeout,this,&CHealthShower::onTick_);
    timer_->start(0);
}

void CHealthShower::addEntity(Entity *entity)
{
    // do nothing if already added (prevent double-listening)
    if (STLWrappers::contains(entities_,entity))
        return;

    // listen for move and health changed signals
    connect(entity,&Entity::healthChanged,this,&CHealthShower::onEntityHealthChanged_);
    connect(entity,&Entity::moved,this,&CHealthShower::onEntityMoved_);

    // listen to map entered/left
    connect(entity,&Entity::mapEntered,this,&CHealthShower::onEntityEntersMap_);
    connect(entity,&Entity::mapLeft,this,&CHealthShower::onEntityLeavesMap_);

    // liste to map visualized/unvisualized by game
    Map* entitysMap = entity->map();
    if (entitysMap){
        connect(entitysMap,&Map::setAsCurrentMap,this,&CHealthShower::onEntitysMapVisualized_);
        connect(entitysMap,&Map ::unsetAsCurrentMap,this,&CHealthShower::onEntitysMapUnVisualized_);
    }

    // listen to dying
    connect(entity,&Entity::dying,this,&CHealthShower::onEntityDying_);

    // create bar
    Bar* bar = new Bar();
    bar->setWidth(40);
    bar->setMinValue(0);
    bar->setMaxValue(entity->maxHealth());
    bar->setCurrentValue(entity->health());
    entityToBar_[entity] = bar;
    Game* entitysGame = nullptr;
    if (entitysMap)
        entitysGame = entitysMap->game();
    if (entitysGame){
        entitysGame->addGui(bar);
        bar->setGuiPos(entitysGame->mapFromMap(entity->pos()));
    }

    // add
    STLWrappers::add(entities_,entity);
}

/// Executed when one of the entitys of the CHealthShower moves.
/// Will update position of the entity's health bar.
void CHealthShower::onEntityMoved_(Entity *sender, QPointF fromPos, QPointF toPos)
{
    Map* entitysMap = sender->map();
    Game* entitysGame = nullptr;
    if (entitysMap)
        entitysGame = entitysMap->game();

    Bar* bar = entityToBar_[sender];
    if (entitysGame)
        bar->setGuiPos(entitysGame->mapFromMap(sender->pos()));
}

/// Executed when the health of one of the entitys of the CHealthShower changes.
/// Will update health bar of the entity.
void CHealthShower::onEntityHealthChanged_(Entity *sender)
{
    Bar* bar = entityToBar_[sender];
    bar->setMinValue(0);
    bar->setMaxValue(sender->maxHealth());
    bar->setCurrentValue(sender->health());
}

/// Executed when one of the entitys of the CHealthShower enters a map.
/// If the entered map is visualized, will make sure its health bar is added to it, other wise
/// will make sure the health bar is removed.
void CHealthShower::onEntityEntersMap_(Entity *sender, Map *mapEntered, Map *oldMap)
{

    disconnect(mapEntered,&Map::setAsCurrentMap,this,&CHealthShower::onEntitysMapVisualized_);
    disconnect(mapEntered,&Map ::unsetAsCurrentMap,this,&CHealthShower::onEntitysMapUnVisualized_);
    connect(mapEntered,&Map::setAsCurrentMap,this,&CHealthShower::onEntitysMapVisualized_);
    connect(mapEntered,&Map ::unsetAsCurrentMap,this,&CHealthShower::onEntitysMapUnVisualized_);

    Game* mapsGame = mapEntered->game();
    Bar* bar = entityToBar_[sender];
    if (mapsGame) {// if the new map is visualized, add gui
        mapsGame->addGui(bar);
        bar->setGuiPos(mapsGame->mapFromScene(sender->pos()));
    }
    else          // other wise, remove it
        mapsGame->removeGui(bar);

}

/// Executed when one of the entitys of the CHealthShower leaves a map.
/// Will make sure the GUI gets removed from the game.
void CHealthShower::onEntityLeavesMap_(Entity *sender, Map *mapLeft)
{
    Game* mapsGame = mapLeft->game();
    if (mapsGame)
        mapsGame->removeGui(entityToBar_[sender]);
}

/// Executed when the map of an entity is visualized.
/// Will make sure its health bar is added to the game.
void CHealthShower::onEntitysMapVisualized_(Map *mapVisualized)
{
    // approach:
    // - go through each entity, find the ones that are in 'mapVisualized', add their bar to game

    Game* g = mapVisualized->game();

    std::vector<Entity*> entitiesInMap;
    for (Entity* e : entities_){
        Map* entitysMap = e->map();
        if (entitysMap == mapVisualized)
            entitiesInMap.push_back(e);
    }

    for (Entity* e:entitiesInMap){
        Bar* bar = entityToBar_[e];
        g->addGui(bar);
        bar->setGuiPos(g->mapFromScene(e->pos()));
    }
}

/// Executed when the map of an entity is unvisualized.
/// Will make sure all the bars of those entities are removed from game.
void CHealthShower::onEntitysMapUnVisualized_(Map *mapUnVisualized)
{
    // approach:
    // - go through entities, find ones in 'mapUnVisualized', remove their bars from game

    Game* g = nullptr;

    std::vector<Entity*> entitiesInMap;
    for (Entity* e : entities_){
        Map* entitysMap = e->map();
        if (entitysMap){
            if (entitysMap == mapUnVisualized)
                entitiesInMap.push_back(e);

            if (entitysMap->game() != nullptr)
                g = entitysMap->game();
        }
    }

    for (Entity* e:entitiesInMap){
        Bar* bar = entityToBar_[e];
        g->removeGui(bar);
    }
}

/// Executed when an entity dies.
/// Cleans up.
void CHealthShower::onEntityDying_(Entity *sender)
{
    // unlisten for move and health changed signals
    disconnect(sender,&Entity::healthChanged,this,&CHealthShower::onEntityHealthChanged_);
    disconnect(sender,&Entity::moved,this,&CHealthShower::onEntityMoved_);

    // unlisten to map entered/left
    disconnect(sender,&Entity::mapEntered,this,&CHealthShower::onEntityEntersMap_);
    disconnect(sender,&Entity::mapLeft,this,&CHealthShower::onEntityLeavesMap_);

    // remove gui
    Map* m = sender->map();
    if (m){
        Game* g = m->game();
        if (g){
            g->removeGui(entityToBar_[sender]);
        }
    }

    STLWrappers::remove(entities_,sender);
    STLWrappers::remove(entityToBar_,sender);
}

/// Executed after all *window* events (mouse/paint/resize etc) have been handled.
/// Will ensure that the health bars are in the proper place relative to their entities.
void CHealthShower::onTick_()
{
    // implementation detail:
    // - when you start() a QTimer with a interval of 0, its call back will be executed after
    //   all the *window* events have been handled.
    // - this makes the GUI "responsive" because window events are given priority, only after
    //   they are finished will this function execute.
    // - the recommended strategy is to have this function do a little bit of work and then return,
    //   then continue your work during the *next* invokation. If you do too much work in one invocation
    //   there will be too much time spent before the next window events have a chance to be executed
    //   thus your application will be unresponsive.

    for (auto entBarPair : entityToBar_){
        Entity* e = entBarPair.first;
        Bar* b = entBarPair.second;

        Map* m = e->map();
        Game* g = nullptr;
        if (m)
            g = m->game();

        if (g){
            double x = e->topLeft().x() + e->boundingRect().width() / 2 - b->getGuiBoundingBox().width() / 2;
            double y = e->topLeft().y();
            b->setGuiPos(g->mapFromMap(QPointF(x,y)));
        }
    }
}
