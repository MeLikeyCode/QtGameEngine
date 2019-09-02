#include "Map.h"

#include "TerrainLayer.h"
#include "Entity.h"
#include "Game.h"
#include "WeatherEffect.h"
#include "Sprite.h"
#include "Sound.h"
#include "PositionalSound.h"
#include "Utilities.h"
#include "EntitySprite.h"
#include "STLWrappers.h"
#include "Gui.h"

using namespace qge;

// TODO remove test
#include <QDebug>

/// Constructs a Map with 50x50, 32x32 sized cells.
Map::Map() : Map(new PathingMap(50,50,32))
{
    // ctor chained
}

/// Constructs a Map with the given number of cells in each direction
/// and the given cell size.
Map::Map(int numCellsX, int numCellsY, double cellSize):
    Map(new PathingMap(numCellsX,numCellsY,cellSize))
{
    // empty
}

Map::Map(PathingMap *pathingMap):
    numCellsWide_(pathingMap->numCellsWide()),
    numCellsLong_(pathingMap->numCellsLong()),
    cellSize_(pathingMap->cellSize()),
    ownPathingMap_(pathingMap),
    overallPathingMap_(new PathingMap(numCellsWide_,numCellsLong_,cellSize_)),
    scene_(new QGraphicsScene(this)),
    game_(nullptr),
    weatherLayer_(new QGraphicsRectItem()),
    guiLayer_(new QGraphicsRectItem()),
    entityLayer_(new QGraphicsRectItem()),
    terrainLayer_(new QGraphicsRectItem())
{
    assert(pathingMap != nullptr); // a Map cannot be constructed with a null PathingMap

    scene_->setSceneRect(0,0,width(),height());

    // add layers in proper order
    scene_->addItem(terrainLayer_);
    scene_->addItem(entityLayer_);
    scene_->addItem(guiLayer_);
    scene_->addItem(weatherLayer_);

    // add a default TerrainLayer
    TerrainLayer* defaultTerrain = new TerrainLayer(width()/256+1, height()/256+1);
    defaultTerrain->fill();
    addTerrainLayer(defaultTerrain);

    // make background black
    QBrush bb;
    bb.setStyle(Qt::SolidPattern);
    bb.setColor(Qt::black);
    scene_->setBackgroundBrush(bb);
}

/// Returns true if the specified pos is in the Map.
bool Map::contains(const QPointF &pos){
    bool botR = pos.x() < qreal(width()) && pos.y() < qreal(height());
    bool topL = pos.x() > 0 && pos.y() > 0;
    return botR && topL;
}

/// Returns true if the Map contains the specified Entity.
bool Map::contains(Entity *entity)
{
    return entities().count(entity);
}

/// Returns the current mouse position in Map coordinates.
/// Throws if the Map isn't the Game's current map.
QPointF Map::getMousePosition()
{
    if (game() == nullptr)
        throw std::logic_error("Map isn't the current map.");

    QPoint pos = game()->getMousePos();
    return game()->mapToScene(pos);
}

/// Returns a reference to the Map's overall PathingMap.
PathingMap &Map::pathingMap(){
    return *overallPathingMap_;
}

/// Adds a PathingMap to the map's overall pathing map.
/// Use removePathingMap() to remove pathing maps from the map's overall pathing map.
void Map::addPathingMap(PathingMap &pm, const QPointF &atPos)
{
    STLWrappers::add(additionalPathingMaps_, &pm, atPos);
}

/// Removes a PathingMap from the map's overall pathing map.
/// use addPathingMap() to add pathing maps to the map's overall pathing map.
void Map::removePathingMap(PathingMap &pm)
{
    STLWrappers::remove(additionalPathingMaps_,&pm);
}

/// Recalculate the pathing map of the map.
/// Takes into account all the pathing maps that have been added as well as the original
/// pathing map that the map was constructed with.
void Map::updatePathingMap()
{
    // approach:
    // - delete old overall pathing map
    // - create a new one (deleting/creating a PathingMap is cheaper than clearing one)
    // - merge own pathing map with overall pathing map
    // - for each additional pathing map:
    //      - merge it with overall pathing map

    delete overallPathingMap_;
    overallPathingMap_ = new PathingMap(numCellsWide_,numCellsLong_,cellSize_);

    overallPathingMap_->addFilling(*ownPathingMap_,QPointF(0,0));

    for (auto& pathingMapPosPair:additionalPathingMaps_){
        PathingMap* pm = pathingMapPosPair.first;
        QPointF pos = pathingMapPosPair.second;
        overallPathingMap_->addFilling(*pm,pos);
    }

    //drawPathingMap(); for debugging
//    drawEntityBBoxes(); for debugging
}

int Map::width() const{
    return ownPathingMap_->width();
}

int Map::height() const{
    return ownPathingMap_->height();
}

QSizeF Map::size() const{
    return QSizeF(width(),height());
}

int Map::numCellsWide() const{
    return numCellsWide_;
}

int Map::numCellsLong() const{
    return numCellsLong_;
}

int Map::cellSize() const{
    return cellSize_;
}

/// Returns the point representing the top left corner of the specified cell.
QPointF Map::cellToPoint(const Node& cell)
{
    return pathingMap().cellToPoint(cell);
}

/// Returns the cell at the specified point.
Node Map::pointToCell(const QPointF &point)
{
    return pathingMap().pointToCell(point);
}

std::unordered_set<Entity *> Map::entities(){
    return entities_;
}

/// Draws the PathingMap for testing purposes. Filled cells are red.
void Map::drawPathingMap(){
    // first, delete all previous drawings
    for (int i = 0, n = drawings_.size(); i < n; ++i){
        scene_->removeItem(drawings_[i]);
        delete drawings_[i];
    }
    drawings_.clear();

    // draw all cells
    for (Node cell:pathingMap().cells()){
        QGraphicsRectItem* rect = new QGraphicsRectItem();
        rect->setRect(0,0,pathingMap().cellSize(),pathingMap().cellSize());
        rect->setPos(cell.x()*pathingMap().cellSize(),cell.y()*pathingMap().cellSize());
        scene_->addItem(rect);
        drawings_.push_back(rect);
        // make empty red
        QPen pen;
        pen.setColor(Qt::red);
        pen.setStyle(Qt::SolidLine);
        rect->setPen(pen);

        // fill the fille ones
        if (pathingMap().filled(cell)){
            QGraphicsRectItem* solidRect = new QGraphicsRectItem();
            solidRect->setRect(0,0,pathingMap().cellSize(),pathingMap().cellSize());
            solidRect->setPos(cell.x()*pathingMap().cellSize(),cell.y()*pathingMap().cellSize());
            scene_->addItem(solidRect);
            drawings_.push_back(solidRect);
            // make filled red
            QBrush brush2;
            brush2.setColor(Qt::red);
            brush2.setStyle(Qt::SolidPattern);
            solidRect->setBrush(brush2);
            solidRect->setOpacity(0.25);
        }
    }

}

void Map::drawEntityBBoxes()
{
    static std::vector<QGraphicsPolygonItem*> polygons;

    for (QGraphicsPolygonItem* p:polygons){
        scene()->removeItem(p);
        delete p;
    }
    polygons.clear();

    for(Entity* e:entities()){
        polygons.push_back(scene()->addPolygon(e->mapToMap(e->boundingRect())));
    }
}

/// Returns the Entities in the specified region.
std::unordered_set<Entity *> Map::entities(const QRectF &inRect){
    QPolygonF inRegionAsPoly(inRect);

    std::unordered_set<Entity*> ents;
    for (Entity* entity:entities()){
        QPolygonF entityBBox = entity->mapToMap(entity->boundingRect());
        if (inRegionAsPoly.intersected(entityBBox).isEmpty() == false)
            ents.insert(entity);
    }

    return ents;
}


/// Returns the Entitys at the specified point.
std::unordered_set<Entity *> Map::entities(const QPointF &atPoint)
{
    // traverse through list of Entities and see if Entity's bounding
    // rect contains the specified point
    std::unordered_set<Entity*> ents;
    for (Entity* entity:entities()){
        // get the Entity's bounding rect
        QPolygonF bRect(entity->mapToMap(entity->boundingRect()));

        // see if the bounding rect contains the point
        if (bRect.containsPoint(atPoint,Qt::FillRule::OddEvenFill))
            ents.insert(entity);
    }

    return ents;
}

/// Returns the Entities in the specified region.
std::unordered_set<Entity *> Map::entities(const QPolygonF &inRegion)
{
    // approach:
    // - 'inRegion' is in scene coordinates
    // - can convert bounding rect of sprite of all entities to scene coordinates
    // - can use qt's collision functionality

    std::unordered_set<Entity*> ents;
    for (Entity* entity:entities()){
        QPolygonF entityBox = entity->mapToMap(entity->boundingRect());
        if (!entityBox.intersected(inRegion).isEmpty())
            ents.insert(entity);
    }

    return ents;
}

/// Returns all the Entities that are colliding with the specified entity.
std::unordered_set<Entity *> Map::entities(Entity *collidingWith)
{
    // approach:
    // - get bbox of 'collidingWith'
    // - return entites colliding with that rect

    QPolygonF bbox = collidingWith->mapToMap(collidingWith->boundingRect());
    std::unordered_set<Entity*> results = entities(bbox);

    results.erase(collidingWith);   // remove entity itself

    return results;
}

/// Returns the Entities in the specified region and in the specified z range.
std::unordered_set<Entity *> Map::entities(const QRectF &inRegion, double zRangeMin, double zRangeMax)
{
    // get all entities in this region
    auto entitiesInRegion = entities(inRegion);

    // filter based on specified z range
    std::unordered_set<Entity*> entities;
    for (Entity* entity:entitiesInRegion){
        double entitysZ = entity->z();
        double entitysZPlusHeight = entitysZ + entity->height();
        if ((entitysZ >= zRangeMin && entitysZ <= zRangeMax) ||
                (entitysZPlusHeight >= zRangeMin && entitysZPlusHeight <= zRangeMax)){
            entities.insert(entity);
        }
    }

    return entities;
}

/// Returns the Entities at the specified point and in the specified z range.
std::unordered_set<Entity *> Map::entities(const QPointF &atPoint, double zRangeMin, double zRangeMax)
{
    // get all entities in at this point
    auto entitiesAtPoint = entities(atPoint);

    // return only ones in valid z range
    std::unordered_set<Entity*> entities;
    for (Entity* entity:entitiesAtPoint){
        double entitysZ = entity->z();
        double entitysZPlusHeight = entitysZ + entity->height();
        if ((entitysZ >= zRangeMin && entitysZ <= zRangeMax) ||
                (entitysZPlusHeight >= zRangeMin && entitysZPlusHeight <= zRangeMax)){
            entities.insert(entity);
        }
    }

    return entities;
}

/// Returns the Entities in the specified region and in the specified z range.
std::unordered_set<Entity *> Map::entities(const QPolygonF &inRegion, double zRangeMin, double zRangeMax)
{
    // get all entities in this region
    auto entitiesInRegion = entities(inRegion);

    // filter based on z range
    std::unordered_set<Entity*> entities;
    for (Entity* entity:entitiesInRegion){
        double entitysZ = entity->z();
        double entitysZPlusHeight = entitysZ + entity->height();
        if ((entitysZ >= zRangeMin && entitysZ <= zRangeMax) ||
                (entitysZPlusHeight >= zRangeMin && entitysZPlusHeight <= zRangeMax)){
            entities.insert(entity);
        }
    }

    return entities;
}

/// Plays the specified sprite's specified animation at the specified position
/// on the map - once.
/// @warning Deletes the sprite after it is finished playing (in other words, Map takes ownership of the sprite).
void Map::playOnce(Sprite *sprite, std::string animationName, int delaybwFramesMS, QPointF atPos)
{
    sprite->setPos(atPos);
    scene()->addItem(sprite);

    QObject::connect(sprite,&Sprite::animationFinished,sprite,&Sprite::deleteLater);

    sprite->play(animationName,1, 1/msToSeconds(delaybwFramesMS));
}

/// Adds the specified WeatherEffect to the Map. Will automatically start the
/// WeatherEffect. A Map can have multiple weather effects active at the same
/// time. You can use WeatherEffect::start() and WeatherEffect::stop() to
/// start/stop a certain WeatherEffect or you can completely remove it by using
/// removeWeatherEffect().
void Map::addWeatherEffect(WeatherEffect& weatherEffect)
{
    // TODO: handle case when weather effect is already in another map

    weatherEffect.map_ = this;
    weatherEffect.start();
    weatherEffects_.insert(&weatherEffect);
}

/// Removes the specified WeatherEffect from the Map.
/// Will automatically stop the WeatherEffect.
/// @see addWeatherEffect()
void Map::removeWeatherEffect(WeatherEffect& weatherEffect)
{
    // cannot remove a weather effect that has not been added
    assert(weatherEffects_.count(&weatherEffect) != 0);

    weatherEffect.stop();
    weatherEffect.map_ = nullptr;
    weatherEffects_.erase(&weatherEffect);
}

/// Adds a PositionalSound to the Map.
/// PositionalSounds adjust their volume based on their distance from the camera.
void Map::addPositionalSound(PositionalSound *sound)
{
    sound->setMap_(this);
}

/// Adds a picture ontop of the terrain at the specified position as a decoration.
void Map::addTerrainDecoration(const QPixmap &picture, const QPointF atPos)
{
    QGraphicsPixmapItem* pmItem = new QGraphicsPixmapItem(picture,terrainLayer_);
    pmItem->setPos(atPos);
}

void Map::addGui(Gui *gui)
{
    gui->getGraphicsItem()->setParentItem(this->guiLayer_);
}

void Map::removeGui(Gui *gui)
{
    gui->getGraphicsItem()->setParentItem(nullptr);
    scene_->removeItem(gui->getGraphicsItem());
}

/// Executed when the game's camera moves around its current map.
/// will emit camMoved() if its current map is this map.
void Map::onCamMoved_(QPointF newCamPos)
{
    if (game()->currentMap() == this)
        emit camMoved(newCamPos);
}

/// Executed when the game changes its current map.
/// If the game changed it to this map, will emit setAsCurrentMap() signal.
/// If the game changed it from this map to another map, will emit
/// unsetAsCurrentMap() signal.
/// If the game changed it from another map to another map, will do nothing.
void Map::onMapChanged_(Map *oldMap, Map *newMap)
{
    if (newMap == this)
        emit setAsCurrentMap(this);

    if (oldMap == this && newMap != this)
        emit unsetAsCurrentMap(this);
}

/// Adds the specified TerrainLayer to the Map.
/// TerrainLayers stack in the order added (the later added, the "topper").
/// @lifetime The Map will uniquely own the lifetime of the specified TerrainLayer.
void Map::addTerrainLayer(TerrainLayer *terrainLayer){
    terrainLayers_.push_back(terrainLayer);
    terrainLayer->parentItem_->setParentItem(terrainLayer_);
}

/// Remove the specified TerrainLayer from the Map.
/// @lifetime You now uniquely own the lifetime of the removed terrain layer.
/// @see addTerrainLayer() terrainLayers()
void Map::removeTerrainLayer(TerrainLayer *terrainLayer)
{
    STLWrappers::remove(terrainLayers_,terrainLayer);
    terrainLayer->parentItem_->setParentItem(nullptr);
    scene_->removeItem(terrainLayer->parentItem_);
}

/// Returns all the TerrainLayers that have been added to the Map.
std::vector<TerrainLayer *> Map::terrainLayers()
{
    return terrainLayers_;
}

/// Adds the specified Entity (and all of its children) to the Map.
/// If the Entity is already in the Map, does nothing.
/// If the Entity is in another Map, it will be removed from that Map first.
void Map::addEntity(Entity *entity){
    // if the Entity is already in the Map, do nothing
    if (contains(entity)) {
        return;
    }

    // if the Entity is in another Map, remove it first
    Map* entitysMap = entity->map();
    if (entitysMap != nullptr && entitysMap != this ){ // TODO: The second check in this "if" is unnecessary, I think!
        entitysMap->removeEntity(entity);
    }

    // add the entity to the list of entities
    entities_.insert(entity);

    // add its sprite to the interal QGraphicsScene
    entity->sprite()->underlyingItem_->setParentItem(entityLayer_);

    // update z
    qreal bot = entity->mapToMap(entity->boundingRect().bottomRight()).y();
    entity->sprite()->underlyingItem_->setZValue(bot); // set z value (lower in map -> draw higher on top)

    // add its children's sprite's as a child of its sprites // TODO: Why is this block needed? I don't think its needed...but double check!
    for (Entity* childEntity:entity->children()){
        childEntity->setParentEntity(entity);
    }

    // update Entity's map_ ptr
    entity->map_ = this;

    // update the PathingMap
    addPathingMap(entity->pathingMap(),entity->mapToMap(entity->pathingMapPos()));
    updatePathingMap();

    // recursively add all child entities
    for (Entity* childEntity:entity->children()){
        addEntity(childEntity);
    }

    // notify that this entity (and all of its child entities) have been added to the map.
    emit entity->mapEntered(entity,this,entitysMap);
    emit entityAdded(this,entity);
}

/// Removes the specified entity (and all of its children) from the map. If the
/// Entity is not in the Map, does nothing.
void Map::removeEntity(Entity *entity)
{
    // Entity not in map
    if (!contains(entity)){
        return;
    }

    // recursively remove its children
    for (Entity* child:entity->children()){
        removeEntity(child);
    }

    // remove from list
    entities_.erase(entity);

    // remove sprite (if it has one)
    EntitySprite* entitysSprite = entity->sprite();
    if (entitysSprite != nullptr){
        entitysSprite->underlyingItem_->setParentItem(nullptr);
        scene()->removeItem(entitysSprite->underlyingItem_);
    }

    // set its internal pointer
    entity->map_ = nullptr;

    // remove the pathing of the Entity
    removePathingMap(entity->pathingMap());
    updatePathingMap();

    // emit entity left map event
    entity->mapLeft(entity,this);
    emit entityRemoved(this,entity);
}

/// Returns (a pointer to) the internal scene.
///
/// This returns a pointer to an internal variable, use cautiously.
QGraphicsScene *Map::scene(){
    return scene_;
}

/// Returns the Game that is currently visualizing the map.
/// If the map isn't currently being visualized, returns nullptr.
Game *Map::game()
{
    return game_;
}

/// Sets the Game that is currently visualizing the Map.
/// If no game is currently visualizing the Map, will be nullptr.
void Map::setGame_(Game *game)
{
    // if the map has a previous game, stop listening to that games events
    if (game_){
        disconnect(game_, &Game::camMoved, this, &Map::onCamMoved_);
        disconnect(game_,&Game::mapChanged,this,&Map::onMapChanged_);
    }

    // set internal game var
    game_ = game;

    // listen to new game
    if (game != nullptr){
        connect(game_, &Game::camMoved, this, &Map::onCamMoved_);
        connect(game, &Game::mapChanged, this, &Map::onMapChanged_);
    }
}
