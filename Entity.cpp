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
#include "EquipableItem.h"
#include "Inventory.h"
#include "Slot.h"
#include <algorithm>

/// Default constructor.
Entity::Entity():
    pathingMap_(1,1,64),            // default 1x1 unfilled (in body) PathingMap
    map_(nullptr),
    children_(),
    parent_(nullptr),
    health_(10),                    // default health of 10
    canOnlyBeDamagedBy_(),
    canBeDamagedByAllExcept_(),
    canOnlyBeDamagedByMode_(false), // by default, can be damaged by all
    groupID_(0),                     // default group id of 0
    isFollowedByCam_(false),
    invulnerable_(false),
    zPos_(0),
    height_(0)
{
    // constructor body
    // = some defaults=
    // default sprite
    Sprite* spr = new Sprite();
    setSprite(spr);
}

/// When an Entity is deleted, it will delete all of its children, and then remove
/// itself from the Map. As each child is deleted, it will delete its own children and then
/// remove itself from the Map.
/// You can kinda see how the "flow" of deletion happens :).
Entity::~Entity()
{
    // recursively delete child Entities
    for (Entity* entity: children()){
        delete entity;
    }

    // if the entity has a parent, remove from parents list of children
    Entity* parentEntity = parent();
    if (parentEntity != nullptr){
        parentEntity->children_.erase(this);
    }

    // if the entity is in a map, remove it
    Map* entitysMap = map();
    if (entitysMap != nullptr){
        entitysMap->removeEntity(this);
    }

    delete sprite_;
}

/// Returns the PathingMap of the Entity.
PathingMap Entity::pathingMap() const{
    return pathingMap_;
}

/// Sets the PathingMap of the Entity.
/// The PathingMap is placed at the specified position relative to the Entity.
void Entity::setPathingMap(const PathingMap &pathingMap, const QPointF& pos){
    pathingMap_ = pathingMap;
    pathingMapPos_ = pos;
}

/// Returns a pointer to the Map that the Entity is in. If the Entity is not
/// in any Maps, returns nullptr.
Map *Entity::map() const{
    return map_;
}

/// Sets the Map that the Entity should be in.
/// Simply delegates to Map::addEntity(Entity*);
/// // TODO: get rid of this function, map->addEntity() is enough
/// @see Map::addEntity(Entity*)
void Entity::setMap(Map *toMap)
{
    toMap->addEntity(this);
}

/// Returns the position of the Entity.
///
/// The position is returned relative to the parent Entity. If there is no
/// parent Entitiy, it is returned relative to the Map.
QPointF Entity::pointPos() const{
    return sprite()->pos();
}

/// Returns the position of the Entity's PathingMap relative to the Entity.
QPointF Entity::pathingMapPos() const
{
    return pathingMapPos_;
}

/// Returns the x position of the Entity relative to the parent, relative to
/// Map if no parent.
double Entity::pointX() const
{
    return pointPos().x();
}

/// Returns the y position of the Entity relative to the parent, relative to
/// Map if no parent.
double Entity::pointY() const
{
    return pointPos().y();
}

/// Returns the z position of the Entity. The z position is the height
/// above level ground that the Entity is at.
double Entity::pointZ() const
{
    return zPos_;
}

/// Returns the height of the Entity.
double Entity::height() const
{
    return height_;
}

/// Sets the position of the Entity.
///
/// The position is relative to the parent Entity. If there is no
/// parent Entitiy, it is relative to the Map.
void Entity::setPointPos(const QPointF &pos){
    sprite()->setPos(pos);

    // if the Entity is in a Map, update the PathingMap
    Map* entitysMap = map();
    if (entitysMap){
        entitysMap->updatePathingMap();

        //if the map is in a game, let map know entity moved (watched-watching pair)
        Game* entitysGame = entitysMap->game();
        if (entitysGame){
            entitysGame->onEntityMoved(this);
        }
    }

    // if followed by the camear, tell game cam to move here
    if (isFollowedByCam()){
        entitysMap->game()->setCenterCamPos(this->pointPos());
    }
}

/// Sets the position of the Entity by moving the specified named position.
void Entity::setPointPos(std::string namedPos, const QPointF &pos)
{
    // move regularly
    setPointPos(pos);

    // shift by amount
    QLineF line(namedPoint("namedPos"),QPointF(0,0));
    double newX = pointPos().x() - line.dx();
    double newY = pointPos().y() - line.dy();
    setPointPos(QPointF(newX,newY));
}

/// Sets the position of the Entity's specific point.
///
/// So if you wanted to move the Entity's 50,50 to somewhere, you would use this
/// function.
void Entity::setPointPos(const QPointF &moveThisPt, const QPointF &toThisPoint)
{
    // move regularly
    setPointPos(toThisPoint);

    // shift back
    QLineF line(moveThisPt,QPointF(0,0));
    double newX = pointPos().x() + line.dx();
    double newY = pointPos().y() + line.dy();
    setPointPos(QPointF(newX,newY));
}

/// Sets the x position of the Entity.
void Entity::setPointX(double x)
{
    QPointF newPoint(x,pointY());
    setPointPos(newPoint);
}

/// Sets the y position of the Entity.
void Entity::setPointY(double y)
{
    QPointF newPoint(pointX(),y);
    setPointPos(newPoint);
}

/// Sets the z position of the Entity.
/// See Entity:pointZ() for more info.
void Entity::setPointZ(double z)
{
    zPos_ = z;

    // scale according to z
    if (sprite_ != nullptr){
        scaleSprite_();
    }
}

/// Sets the height of the Entity.
void Entity::setHeight(double height)
{
    height_ = height;
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

    // set scaling of the new sprite
    scaleSprite_();
}

/// Returns the Entity's Sprite. If the Entity does not have a sprite,
/// returns nullptr.
Sprite *Entity::sprite() const{
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
/// // TODO: move out into a non member utility function
/// @see PathingMap::canFit(PathingMap,QPointF).
bool Entity::canFit(const QPointF &atPos)
{
    // if the point is out of map, return false
    if (!map_->contains(atPos)){
        return false;
    }

    return map()->pathingMap().canFit(pathingMap(),atPos);
}

/// Returns the children of this Entity.
std::unordered_set<Entity *> Entity::children()
{
    return children_;
}

/// Sets the parent of this Entity.
/// When a parent Entity moves, rotates, gets added to a Map, gets removed
/// from a Map, or gets deleted, so do all of its children.
void Entity::setParentEntity(Entity *parent)
{
    // if nullptr passed in, make sure Entity has no parent
    if (parent == nullptr){
        if (parent_ != nullptr){
            parent_->children_.erase(this);
            parent_ = nullptr;
        }
        sprite()->setParentItem(nullptr);
        return;
    }

    // if parent passed in, set the parent of the Entity
    if (parent != nullptr){
        // if already has a parent, remove parent first
        if (parent_ != nullptr){
            setParentEntity(nullptr);
        }
        // set new parent
        parent_ = parent;
        parent_->children_.insert(this);
        sprite()->setParentItem(parent->sprite());
    }
}

/// Returns the Entity's parent.
Entity *Entity::parent()
{
    return parent_;
}

/// Maps a point from local (Entity) coordinates to the Map (scene) coordinates.
QPointF Entity::mapToMap(const QPointF &point) const
{
    return sprite()->mapToScene(point);
}

/// Names the specified point (so it can be retrieved with a name).
/// The point is in local (Entity) coordinates.
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

// Sets the point the Entity should rotate about.
void Entity::setRotationPoint(QPointF point)
{
    assert(sprite() != nullptr);  // needs a sprite to have a rotation point
    sprite()->setTransformOriginPoint(point);
}

/// Sets the health of the entity, if set below 0, entity dies.
void Entity::setHealth(double health)
{
    health_ = health;

    if (health_ < 0){
        map()->removeEntity(this);
        delete this;
    }
}

double Entity::health()
{
    return health_;
}

/// Attempts to damage the specified Entity. Every Entity can specifiy,
/// what can and can't damage them.
void Entity::damage(Entity *entity, double amount)
{
    // if can't damage the entity, return
    if (!entity->canBeDamagedBy(this) || entity->isInvulnerable() ){
        return;
    }

    // can be damaged
    entity->setHealth(entity->health() - amount);
}

/// True sets is so that the entity can only be damaged by entites specified
/// in the canOnlyBeDamagedBy_ set.
/// False sets it so that the entity can be damaged by all except those
/// specified in the canBeDamagedByAllExcept_ set.
void Entity::setCanOnlyBeDamagedBy(bool tf)
{
    canOnlyBeDamagedByMode_ = tf;
}

/// Adds a type that this Entity can be damaged by.
void Entity::addCanBeDamagedBy(std::type_index typeOfEntity)
{
    canOnlyBeDamagedBy_.insert(typeOfEntity);
}

void Entity::addCannotBeDamagedBy(std::type_index typeOfEntity)
{
    canBeDamagedByAllExcept_.insert(typeOfEntity);
}

/// Returns true if the Entity can be damaged by the specified entity.
bool Entity::canBeDamagedBy(Entity *entity)
{
    // entity is not in the list of thigns allowed to damage
    if (canOnlyBeDamagedByMode_ && canOnlyBeDamagedBy_.count(std::type_index(typeid(*entity))) == 0){
        return false;
    }
    // entity is in the list of things not allowed to domage
    if (!canOnlyBeDamagedByMode_ && canBeDamagedByAllExcept_.count(std::type_index(typeid(*entity))) != 0){
        return false;
    }

    // entity should be able to dmage
    return true;
}

/// Returns true if the Entity is invulnerable. An invulnerable Entity cannot
/// be damaged.
bool Entity::isInvulnerable()
{
    return invulnerable_;
}

/// Pass true to make the Entity invulnerable, false to make it vulnearable.
/// An invulnerable Entity cannot be damaged.
void Entity::setInvulnerable(bool tf)
{
    invulnerable_ = tf;
}

void Entity::setGroupID(int id)
{
    groupID_ = id;
}

int Entity::groupID()
{
    return groupID_;
}

/// Returns true if the Entity is followed by the camera.
/// If an Entity is followed by the camera, then every time that Entity's
/// position is changed, the camera goes to that new position.
bool Entity::isFollowedByCam()
{
    return isFollowedByCam_;
}

/// Sets weather the Entity should be followed by the camera or not.
/// @see Entity::isFollowedByCam()
void Entity::setFollowedByCam(bool tf)
{
    isFollowedByCam_ = tf;
}

void Entity::scaleSprite_()
{
    assert(sprite_ != nullptr);

    double scaleFactor = 1.0 + zPos_ / 100.0; // scale up by z%
    int newWidth = sprite_->size().width() * (scaleFactor);
    int newHeight = sprite_->size().height() * (scaleFactor);
    sprite_->setSize(newWidth,newHeight);
}
