#include "Entity.h"
#include <string>
#include <cassert>
#include "Map.h"
#include "Sprite.h"
#include <QDebug> // TODO: remove, test
#include <QGraphicsScene>
#include "CountExpiringTimer.h"
#include <QLineF>
#include "Game.h"
#include "MoveRelativeToScreen.h"

/// Default constructor.
Entity::Entity():
    pathingMap_(1,1,63)    // default 1x1 filled (in body) PathingMap
{
    // constructor body
    // = some defaults=
    map_ = nullptr;
    sprite_ = nullptr;

    // default sprite
    Sprite* spr = new Sprite();
    setSprite(spr);
}

/// When an Entity is deleted, it will delete all of its child entities.
Entity::~Entity()
{
    for (Entity* entity: children_){
        delete entity;
    }
}

/// Returns the PathingMap of the Entity.
PathingMap Entity::pathingMap() const{
    return pathingMap_;
}

/// Sets the PathingMap of the Entity.
void Entity::setPathingMap(const PathingMap &pathingMap){
    pathingMap_ = pathingMap;
}

/// Returns a pointer to the Map that the Entity is in.
Map *Entity::map() const{
    // make sure it has a map
    assert(map_);

    return map_;
}

/// Sets the map that the Entity is in.
void Entity::setMap(Map *toMap)
{
    map_ = toMap;
}

/// Returns the position of the Entity.
///
/// The position is returned relative to the parent Entity. If there is no
/// parent Entitiy, it is returned relative to the Map.
QPointF Entity::pointPos() const{
    return sprite()->pos();
}

/// Sets the position of the Entity.
///
/// The position is relative to the parent Entity. If there is no
/// parent Entitiy, it is relative to the Map.
void Entity::setPointPos(const QPointF &pos){
    sprite()->setPos(pos);
}

/// Returns the cell that the Entity is in.
Node Entity::cellPos(){
    return map()->pathingMap().pointToCell(pointPos());
}

/// Sets the Sprite of the Entity. Removes (but does not delete) the old Sprite.
void Entity::setSprite(Sprite *sprite){
    // set all children's sprite's parent to new sprite
    for (Entity* child: children()){
        child->sprite()->setParentItem(sprite);
    }

    // if the Entity is already in a map
    if (map_){
        // set the position of the new sprite to the pos of the old sprite
        sprite->setPos(sprite_->pos());

        // remove old sprite/add new sprite
        map()->scene()->removeItem(sprite_);
        map()->scene()->addItem(sprite);
    }

    // set internal sprite_ pointer to the new sprite
    sprite_ = sprite;

}

/// Returns the Entity's Sprite.
Sprite *Entity::sprite() const{
    // make sure the Entity has a sprite
    assert(sprite_);

    return sprite_;
}

QRectF Entity::boundingRect()
{
    return sprite()->boundingRect();
}

/// Returns the current angle the Entity is facing in degrees.
int Entity::facingAngle()
{
    return sprite()->rotation();
}

/// Sets the facing angle of the entity.
void Entity::setFacingAngle(double angle)
{
    sprite()->setRotation(angle);
}

/// Instantly moves the Entity to the specified cell in the Map.
void Entity::setCellPos(const Node &cell){
    // make sure the entity has a Map
    assert(map_);

    // clear pathing map
    // disablePathingMap(); // only static entities have pathing maps

    // move to new region
    QPointF newPos = map()->pathingMap().cellToPoint(cell);
    setPointPos(newPos);

    // enable pathing map
    // enablePathingMap();
}

/// Returns true if the Entity can fit at the specified point.
///
/// Takes the Entity's bounding rect and sees if it can fit in the specified point.
/// I.e if it doesn't intersect with any filled cells in the Map's PathingMap.
bool Entity::canFit(const QPointF &atPos)
{
    // get the bounding rect
    int CUSHION = 20;

    QRectF checkRegion = boundingRect();
    checkRegion.moveTo(atPos);
    checkRegion.setWidth(checkRegion.width() - CUSHION);
    checkRegion.setHeight(checkRegion.height() -CUSHION);
    checkRegion.moveTopLeft(QPointF(checkRegion.x() + CUSHION/2,checkRegion.y() + CUSHION/2));

    // see if that region is free in the map
    return map()->pathingMap().free(checkRegion);

}

/// Disables the Entity's PathingMap.
///
/// Essentially, clears the region in the Map that corresponds to the Entity's
/// PathingMap.
void Entity::disablePathingMap(){
    QRectF clearRegion;
    clearRegion.setTopLeft(pointPos());
    clearRegion.setWidth(pathingMap().width());
    clearRegion.setHeight(pathingMap().height());
    std::vector<Node> cellsInRegion = map()->pathingMap().cells(clearRegion);
    for (Node cell:cellsInRegion){
        map()->pathingMap().unfill(cell);
    }

    // TODO test remove
    map()->drawPathingMap();
}

/// Enables the Entity's PathingMap.
///
/// Essentially, "draws" the PathingMap of the Entity on the Map's PathingMap.
void Entity::enablePathingMap(){
    map()->pathingMap().setFilling(pointPos(),pathingMap());

    // TODO test, remove
    map()->drawPathingMap();
}

/// Returns the children of this Entity.
std::vector<Entity *> Entity::children() const
{
    return children_;
}

/// Sets the parent of this Entity.
///
/// When a parent Entity moves or rotates, all of its children do too!
void Entity::setParentEntity(Entity *parent)
{
    parent_ = parent;
    parent->children().push_back(this);

    // make sprite set parent too
    sprite()->setParentItem(parent->sprite());
}

/// Maps a point from local (Entity) coordinates to the Map (scene) coordinates.
QPointF Entity::mapToMap(const QPointF &point) const
{
    return sprite()->mapToScene(point);
}

/// Names the specified point (so it can be retrieved with a name).
void Entity::addNamedPoint(const QPointF &point, std::string name)
{
    namedPoints_[name] = point;
}

/// Returns the point represented by the specified name.
QPointF Entity::namedPoint(std::string name)
{
    // make sure the points exists
    assert(namedPoints_.count(name));

    return namedPoints_[name];
}
