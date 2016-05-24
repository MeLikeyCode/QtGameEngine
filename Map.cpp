#include "Map.h"
#include "Terrain.h"
#include "DynamicEntity.h"
#include <cassert>
#include <QBrush>
#include <QGraphicsScene>
#include "Game.h"

// TODO remove test
#include <QDebug>

Map::Map(int numCellsWide, int numCellsLong, int cellSize):
    numCellsWide_(numCellsWide),
    numCellsLong_(numCellsLong),
    cellSize_(cellSize),
    pathingMap_(numCellsWide,numCellsLong,cellSize)
{
    // constructor body
    scene_ = new QGraphicsScene();

    // calculate width and height
    width_ = numCellsWide * cellSize;
    height_ = numCellsLong * cellSize;
    scene_->setSceneRect(0,0,width_,height_);

    // set up a default terrain (grass.png)
    int TILE_SIZE = 256;
    terrain_ = new Terrain(width()/TILE_SIZE+1,height()/TILE_SIZE+1,
                           TILE_SIZE,TILE_SIZE);
    terrain_->fill(QPixmap(":resources/graphics/terrain/grass.png"));
    setTerrain(terrain_);
}

/// Returns true if the specified pos is in the Map.
bool Map::contains(const QPointF &pos){
    return pos.x() < qreal(width()) && pos.y() < qreal(height());
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

int Map::width() const{
    return width_;
}

int Map::height() const{
    return height_;
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

/// Sets the Terrain of the Map.
void Map::setTerrain(Terrain *to){
    terrain_ = to;

    // add the parent terrain to the map's scene
    scene()->addItem(to->parentItem());

}

/// Adds the specified Entity to the Map and updates the PathingMap.
/// If the Entity is already in the Map, does nothing.
/// If the Entity is in another Map, it will be removed from that Map first.
/// All of the Entities children are added to this map as well.
void Map::addEntity(Entity *entity){
    // if the Entity is already in the Map, do nothing
    if (contains(entity)) {
        return;
    }

    // if the Entity is in another Map, remove it first
    if (entity->map() != nullptr ){
        entity->map()->removeEntity(entity);
    }

    // add the entity to the list of entities
    entities_.insert(entity);

    // add its sprite to the interal QGraphicsScene
    scene_->addItem(entity->sprite());

    // update Entity's map_ ptr
    entity->map_ = this;

    // update the PathingMap
    // entity->enablePathingMap(); // TODO work on

    // add all child entities
    for (Entity* childEntity:entity->children()){
        addEntity(childEntity);
    }

    drawPathingMap(); // TODO test remove
}

/// Removes the specified entity (and all of its children) from the map. If the
/// Entity is not in the Map, does nothing.
void Map::removeEntity(Entity *entity)
{
    // Entity not in map
    if (!contains(entity)){
        return;
    }

    // remove from list
    entities_.erase(entity);

    // remove sprite (if it has one)
    if (entity->sprite() != nullptr){
        scene()->removeItem(entity->sprite());
    }

    // set its internal pointer
    entity->map_ = nullptr;

    // recursively, remove its children as well
    for (Entity* child:entity->children()){
        removeEntity(child);
    }

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
    // make sure map has a game
    assert(game_);

    return game_;
}

void Map::setGame(Game *game)
{
    game_ = game;
}
