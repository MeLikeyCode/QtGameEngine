#include "Map.h"

#include "TerrainLayer.h"
#include "Entity.h"
#include <cassert>
#include <QBrush>
#include <QGraphicsScene>
#include "Game.h"
#include "WeatherEffect.h"
#include <QGraphicsOpacityEffect>
#include "Sprite.h"

// TODO remove test
#include <QDebug>

Map::Map(PathingMap pathingMap):
    numCellsWide_(pathingMap.numCellsWide()),
    numCellsLong_(pathingMap.numCellsLong()),
    cellSize_(pathingMap.cellSize()),
    pathingMap_(pathingMap),
    scene_(new QGraphicsScene(this)),
    weather_(nullptr),
    game_(nullptr)
{
    scene_->setSceneRect(0,0,width(),height());

    // add a default TerrainLayer
    TerrainLayer* defaultTerrain = new TerrainLayer(width()/256+1,
                                                    height()/256+1,
                                                    QPixmap(":resources/graphics/terrain/grass.png"));
    defaultTerrain->fill();
    addTerrainLayer(defaultTerrain);

    // make background black
    QBrush bb;
    bb.setStyle(Qt::SolidPattern);
    bb.setColor(Qt::black);
    scene_->setBackgroundBrush(bb);

    // add fading rects
    setFadingBorder_();
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
QPointF Map::getMousePosition()
{
    QPoint pos = game()->getMousePos();
    return game()->mapToScene(pos);
}

/// Returns a reference to the Map's PathingMap.
PathingMap &Map::pathingMap(){
    return pathingMap_;
}

/// Updates the PathingMap with the positions of all entities.
void Map::updatePathingMap()
{
    // approach:
    // - clear pathing map (make it all unfil)
    // - traverse through entities, put thier pathingmap in pathing map

    // unfill the entire PathingMap
    pathingMap().unfill();

    // fill each Entity's pathing map
    for (Entity* entity:entities()){
        pathingMap().addFilling(entity->pathingMap(),entity->pointPos() + entity->pathingMapPos());
    }

    //drawPathingMap(); // TODO: remove, test
}

int Map::width() const{
    return pathingMap_.width();
}

int Map::height() const{
    return pathingMap_.height();
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

/// Returns the distance (in pixels) b/w the two Entities.
double Map::distance(Entity *e1, Entity *e2){
    QLineF line(QPointF(e1->pointPos()),QPointF(e2->pointPos()));
    return line.length();
}

/// Returns (a pointer to) the closest Entity to the specified point.
Entity *Map::closest(const QPointF &point){
    // assume the first Entity in the list is the closest
    Entity* closestEntity = *entities().begin();
    double closestDistance = QLineF(closestEntity->pointPos(),point).length();
    // try to find a closer one
    for (Entity* entity:entities()){
        double thisDistance = QLineF(entity->pointPos(),point).length();
        if (thisDistance < closestDistance){
            closestDistance = thisDistance;
            closestEntity = entity;
        }
    }

    // return the closest Entity
    return closestEntity;
}

/// Returns the Entities in the specified region.
std::unordered_set<Entity *> Map::entities(const QRectF &inRegion){
    std::unordered_set<Entity*> ents;
    for (Entity* entity:entities()){
        if (inRegion.contains(entity->pointPos())){
            ents.insert(entity);
        }
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
        QRectF bRect(entity->boundingRect());
        bRect.moveTo(entity->mapToMap(QPointF(0,0)));

        // see if the bounding rect contains the point
        if (bRect.contains(atPoint)){
            ents.insert(entity);
        }
    }

    return ents;
}

/// Returns the Entities in the specified region.
std::unordered_set<Entity *> Map::entities(const QPolygonF &inRegion)
{
    std::unordered_set<Entity*> ents;
    for (Entity* entity:entities()){
        if (inRegion.containsPoint(entity->pointPos(),Qt::OddEvenFill)){
            ents.insert(entity);
        }
    }

    return ents;
}

/// Returns all the Entities that are colliding with the specified entity.
std::unordered_set<Entity *> Map::entities(Entity *collidingWith)
{
    // approach:
    // - get bbox of colliding entity, move it to location of entity
    // - return entites colliding with that rect

    QRectF bbox = collidingWith->boundingRect();
    bbox.moveTopLeft(collidingWith->pointPos());
    std::unordered_set<Entity*> results = entities(bbox);

    results.erase(collidingWith);   // remove entity itself

    return results;
}

/// Returns the Entities in the specified region and in the specified z range.
std::unordered_set<Entity *> Map::entities(const QRectF &inRegion, double zRangeMin, double zRangeMax)
{
    // get all entities in this region
    auto entitiesInRegion = entities(inRegion);

    std::unordered_set<Entity*> entities;
    for (Entity* entity:entitiesInRegion){
        double entitysZ = entity->pointZ();
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
        double entitysZ = entity->pointZ();
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

    std::unordered_set<Entity*> entities;
    for (Entity* entity:entitiesInRegion){
        double entitysZ = entity->pointZ();
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
/// @warning Deletes the sprite after it is finished playing.
/// TODO: don't delete the sprite (create a copy, delete the copy, or something...)
void Map::playOnce(Sprite *sprite, std::string animationName, int delaybwFramesMS, QPointF atPos)
{
    sprite->setPos(atPos);
    scene()->addItem(sprite);

    QObject::connect(sprite,&Sprite::animationFinished,sprite,&Sprite::deleteLater);

    sprite->play(animationName,1,delaybwFramesMS);
}

/// Sets the weather effect for Map. Pass in nullptr for no weather.
/// If there is already a weather effect, will stop it first.
void Map::setWeatherEffect(WeatherEffect *weather)
{
    if (weather_){
      weather_->stop();
    }

    weather_ = weather;

    if (weather){
        weather->map_ = this;
        weather->start();
    }
}

/// Returns the WeatherEffect of the Map. Returns nullptr if the Map has no
/// WeatherEffect. You can start and stop the returned WeatherEffect via
/// WeatherEffect::start()/WeatherEffect::stop().
WeatherEffect *Map::weatherEffect()
{
    return weather_;
}

/// Sets a fading border around the map.
void Map::setFadingBorder_()
{
    const int EDGE = 0;
    const int WIDTH = 100;
    const int R_WIDTH = 500;

    QBrush b;
    b.setStyle(Qt::SolidPattern);
    b.setColor(Qt::black);

    // top
    QLinearGradient topGradient;
    topGradient.setStart(0,R_WIDTH);
    topGradient.setFinalStop(0,R_WIDTH-WIDTH);
    topGradient.setColorAt(0,Qt::transparent);
    topGradient.setColorAt(1,Qt::black);

    QGraphicsOpacityEffect* topOpacity = new QGraphicsOpacityEffect();
    topOpacity->setOpacity(1);
    topOpacity->setOpacityMask(topGradient);

    QGraphicsRectItem* topRect = new QGraphicsRectItem();
    topRect->setRect(0,0,width()-EDGE*2,R_WIDTH);
    topRect->setPos(EDGE,0-R_WIDTH);
    topRect->setBrush(b);
    topRect->setGraphicsEffect(topOpacity);

    topRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(topRect);

    // bottom
    QLinearGradient bottomGradient;
    bottomGradient.setStart(0,0);
    bottomGradient.setFinalStop(0,WIDTH);
    bottomGradient.setColorAt(0,Qt::transparent);
    bottomGradient.setColorAt(1,Qt::black);

    QGraphicsOpacityEffect* bottomOpacity = new QGraphicsOpacityEffect();
    bottomOpacity->setOpacity(1);
    bottomOpacity->setOpacityMask(bottomGradient);

    QGraphicsRectItem* bottomRect = new QGraphicsRectItem();
    bottomRect->setRect(0,0,width()-EDGE*2,R_WIDTH);
    bottomRect->setPos(EDGE,height());
    bottomRect->setBrush(b);
    bottomRect->setGraphicsEffect(bottomOpacity);

    bottomRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(bottomRect);

    // left
    QLinearGradient leftGradient;
    leftGradient.setStart(R_WIDTH,0);
    leftGradient.setFinalStop(R_WIDTH-WIDTH,0);
    leftGradient.setColorAt(0,Qt::transparent);
    leftGradient.setColorAt(1,Qt::black);

    QGraphicsOpacityEffect* leftOpacity = new QGraphicsOpacityEffect();
    leftOpacity->setOpacity(1);
    leftOpacity->setOpacityMask(leftGradient);

    QGraphicsRectItem* leftRect = new QGraphicsRectItem();
    leftRect->setRect(0,0,R_WIDTH,height()-EDGE*2);
    leftRect->setPos(0-R_WIDTH,0+EDGE);
    leftRect->setBrush(b);
    leftRect->setGraphicsEffect(leftOpacity);

    leftRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(leftRect);

    // right
    QLinearGradient rightGradient;
    rightGradient.setStart(0,0);
    rightGradient.setFinalStop(WIDTH,0);
    rightGradient.setColorAt(0,Qt::transparent);
    rightGradient.setColorAt(1,Qt::black);

    QGraphicsOpacityEffect* rightOpacity = new QGraphicsOpacityEffect();
    rightOpacity->setOpacity(1);
    rightOpacity->setOpacityMask(rightGradient);

    QGraphicsRectItem* rightRect = new QGraphicsRectItem();
    rightRect->setRect(0,0,R_WIDTH,height()-EDGE*2);
    rightRect->setPos(width(),0+EDGE);
    rightRect->setBrush(b);
    rightRect->setGraphicsEffect(rightOpacity);

    rightRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(rightRect);

    // top left
    QRadialGradient tlGradient;
    tlGradient.setCenter(R_WIDTH, R_WIDTH);
    tlGradient.setFocalPoint(tlGradient.center());
    tlGradient.setRadius(R_WIDTH);
    tlGradient.setColorAt(0,Qt::transparent);
    tlGradient.setColorAt(0.2,Qt::black);

    QGraphicsOpacityEffect* tlOpacity = new QGraphicsOpacityEffect();
    tlOpacity->setOpacity(1);
    tlOpacity->setOpacityMask(tlGradient);

    QGraphicsRectItem* tlRect = new QGraphicsRectItem();
    tlRect->setRect(0,0,R_WIDTH,R_WIDTH);
    tlRect->setPos(0-R_WIDTH,0-R_WIDTH);
    tlRect->setBrush(b);
    tlRect->setGraphicsEffect(tlOpacity);

    tlRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(tlRect);

    // top right
    QRadialGradient trGradient;
    trGradient.setCenter(0, R_WIDTH);
    trGradient.setFocalPoint(trGradient.center());
    trGradient.setRadius(R_WIDTH);
    trGradient.setColorAt(0,Qt::transparent);
    trGradient.setColorAt(0.2,Qt::black);

    QGraphicsOpacityEffect* trOpacity = new QGraphicsOpacityEffect();
    trOpacity->setOpacity(1);
    trOpacity->setOpacityMask(trGradient);

    QGraphicsRectItem* trRect = new QGraphicsRectItem();
    trRect->setRect(0,0,R_WIDTH,R_WIDTH);
    trRect->setPos(width(),0-R_WIDTH);
    trRect->setBrush(b);
    trRect->setGraphicsEffect(trOpacity);

    trRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(trRect);

    // bottom left
    QRadialGradient blGradient;
    blGradient.setCenter(R_WIDTH, 0);
    blGradient.setFocalPoint(blGradient.center());
    blGradient.setRadius(R_WIDTH);
    blGradient.setColorAt(0,Qt::transparent);
    blGradient.setColorAt(0.2,Qt::black);

    QGraphicsOpacityEffect* blOpacity = new QGraphicsOpacityEffect();
    blOpacity->setOpacity(1);
    blOpacity->setOpacityMask(blGradient);

    QGraphicsRectItem* blRect = new QGraphicsRectItem();
    blRect->setRect(0,0,R_WIDTH,R_WIDTH);
    blRect->setPos(0-R_WIDTH,height());
    blRect->setBrush(b);
    blRect->setGraphicsEffect(blOpacity);

    blRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(blRect);

    // bottom right
    QRadialGradient brGradient;
    brGradient.setCenter(0, 0);
    brGradient.setFocalPoint(brGradient.center());
    brGradient.setRadius(R_WIDTH);
    brGradient.setColorAt(0,Qt::transparent);
    brGradient.setColorAt(0.2,Qt::black);

    QGraphicsOpacityEffect* brOpacity = new QGraphicsOpacityEffect();
    brOpacity->setOpacity(1);
    brOpacity->setOpacityMask(brGradient);

    QGraphicsRectItem* brRect = new QGraphicsRectItem();
    brRect->setRect(0,0,R_WIDTH,R_WIDTH);
    brRect->setPos(width(),height());
    brRect->setBrush(b);
    brRect->setGraphicsEffect(brOpacity);

    brRect->setZValue(Z_VALUES::BORDER_Z_VALUE);
    scene_->addItem(brRect);
}

/// Adds the specified TerrainLayer to the Map.
/// TerrainLayers stack in the order added (the later added, the "topper").
void Map::addTerrainLayer(TerrainLayer *terrainLayer){
    terrainLayers_.push_back(terrainLayer);

    // add the parent terrain to the map's scene
    terrainLayer->parentItem_->setZValue(Z_VALUES::TERRAIN_Z_VALUE);
    scene()->addItem(terrainLayer->parentItem_);

}

/// Adds the specified Entity (and all of its children) to the Map and updates
/// the PathingMap.
/// If the Entity is already in the Map, does nothing.
/// If the Entity is in another Map, it will be removed from that Map first.
void Map::addEntity(Entity *entity){
    // if the Entity is already in the Map, do nothing
    if (contains(entity)) {
        return;
    }

    // if the Entity is in another Map, remove it first
    Map* entitysMap = entity->map();
    if (entitysMap != nullptr && entitysMap != this ){
        entitysMap->removeEntity(entity);
    }

    // add the entity to the list of entities
    entities_.insert(entity);

    // add its sprite to the interal QGraphicsScene
    scene_->addItem(entity->sprite());

    // set its z value
    entity->sprite()->setZValue(Z_VALUES::ENTITY_Z_VALUE);

    // add its children's sprite's as a child of its sprites
    for (Entity* childEntity:entity->children()){
        childEntity->setParentEntity(entity);
    }

    // update Entity's map_ ptr
    entity->map_ = this;

    // update the PathingMap
    updatePathingMap();

    // recursively add all child entities
    for (Entity* childEntity:entity->children()){
        addEntity(childEntity);
    }

    // notify that this entity (and all of its child entities) have been added to the map.
    emit entity->mapEntered(entity,this,entitysMap);
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
    Sprite* entitysSprite = entity->sprite();
    if (entitysSprite != nullptr){
        scene()->removeItem(entitysSprite);
    }

    // set its internal pointer
    entity->map_ = nullptr;

    // TODO: remove the leftover pathing of the Entity
}

/// Returns (a pointer to) the internal scene.
///
/// This returns a pointer to an internal variable, use cautiously.
QGraphicsScene *Map::scene(){
    return scene_;
}

Game *Map::game()
{
    return game_;
}

void Map::setGame(Game *game)
{
    game_ = game;
}
