#include "Entity.h"

#include <string>
#include <cassert>
#include "Map.h"
#include "Sprite.h"
#include <QGraphicsScene>
#include <QLineF>
#include "Game.h"
#include "EquipableItem.h"
#include "Inventory.h"
#include "Slot.h"
#include <algorithm>

/// Constructs a default entity.
Entity::Entity():
    pathingMap_(1,1,64),            // default 1x1 unfilled (in body) PathingMap
    map_(nullptr),
    sprite_(new Sprite()),
    children_(),
    parent_(nullptr),
    health_(10),                    // default health of 10
    canOnlyBeDamagedBy_(),
    canBeDamagedByAllExcept_(),
    canOnlyBeDamagedByMode_(false), // by default, can be damaged by all
    groupNumber_(0),                     // default group id of 0
    invulnerable_(false),
    zPos_(0),
    height_(0),
    inventory_(new Inventory()),
    speed_(250),
    facingAngle_(0),
    rotationSpeed_(360),
    zValue_(0)
{
    inventory_->entity_ = this;
}

/// When an Entity is deleted, it will delete all of its children, and then
/// remove itself from the Map (if in one). As each child is deleted, it will
/// delete its own children and then remove itself from the Map. You can kinda
/// see how the "flow" of deletion happens :). The "deepest" child is deleted
/// and removed from the Map first.
Entity::~Entity()
{
    emit dying(this); // let listeners know this entity is about to die

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
    return currentPos_;
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
    // set internal variable
    currentPos_ = pos;

    // if has a sprite, set position of sprite
    Sprite* entitysSprite = sprite();
    if (entitysSprite != nullptr)
        entitysSprite->setPos(pos);

    // if the Entity is in a Map, update the PathingMap
    Map* entitysMap = map();
    if (entitysMap){
        entitysMap->updatePathingMap();

        //if the map is in a game, let map know entity moved (watched-watching pair)
        // TODO: remove this, instead have game listen to when entites move
        Game* entitysGame = entitysMap->game();
        if (entitysGame){
            entitysGame->onEntityMoved(this);
        }

        // if collided with something, emit
        std::unordered_set<Entity*> collidingEntities = entitysMap->entities(this);
        for (Entity* entity: collidingEntities){
            emit collided(this,entity);
        }
    }

    // if actually moved (i.e. last position is different than current pos),
    // emit signal
    if (lastPos_ != pointPos()){
        QPointF lastPosCpy = lastPos_;
        lastPos_ = pointPos(); // update last position
        emit moved(this,lastPosCpy,pointPos());
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

/// Moves the entity by the specified dx,dy.
void Entity::moveBy(double dx, double dy)
{
    setPointPos(QPointF(pointX() + dx, pointY() + dy));
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

/// Sets the Sprite of the Entity. This does not *delete* the old sprite, it simply
/// sets the current art to the specified sprite.
void Entity::setSprite(Sprite *sprite){
    // set all children's sprite's parent to new sprite
    for (Entity* child: children()){
        child->sprite()->setParentItem(sprite);
    }

    // make sure the new sprite is positioned correctly
    sprite->setPos(currentPos_);

    // if the Entity is already in a map
    if (map_){
        // remove old sprite/add new sprite
        map()->scene()->removeItem(sprite_);
        map()->scene()->addItem(sprite);
    }

    // set internal sprite_ pointer to the new sprite
    sprite_ = sprite;

    // set scaling of the new sprite
    scaleSprite_();

    // set the zvalue of the new sprite
    sprite->setZValue(zValue());
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
    return facingAngle_;
}

/// Sets the facing angle of the entity.
void Entity::setFacingAngle(double angle)
{
    facingAngle_ = angle;
}

/// Sets the z value of the Entity. Entities with a higher z value are drawn ontop of entities with a lower z value.
void Entity::setZValue(double zValue)
{
    zValue_ = zValue; // update internal variable

    // if has a sprite, update sprites z value
    if (sprite() != nullptr){
        sprite()->setZValue(zValue);
    }
}

/// Returns the z value of the Entity. See setZValue() for more info.
double Entity::zValue()
{
    return zValue_;
}

/// Instantly moves the Entity to the specified cell in the Map.
void Entity::setCellPos(const Node &cell){
    // make sure the entity has a Map
    assert(map_);

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

/// Sets the speed that the entity should generally travel at (in pixels per second).
/// Note that entity controllers often use this variable to determine how to move the
/// entity, so this speed may not be EXACTLY what the actual movement speed of the
/// entity ends up being.
void Entity::setSpeed(double speed)
{
    speed_ = speed;
}

/// Returns the speed of the Entity.
/// @see setSpeed()
double Entity::speed()
{
    return speed_;
}

/// Sets the speed at which this entity should in general rotate at, in degrees
/// per second.
/// Note that entity controllers often use this variable to determine how to rotate
/// the entity, so this rotation speed may not be exactly what the actual ends up being.
/// (that is up to the entity controllers).
void Entity::setRotationSpeed(double rotationSpeed)
{
    rotationSpeed_ = rotationSpeed;
}

/// Returns the rotation speed of the entity.
/// @see setRotationSpeed()
double Entity::rotationSpeed()
{
    return rotationSpeed_;
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
        Map* entitysMap = map();
        if (entitysMap != nullptr)
            entitysMap->removeEntity(this);

        deleteLater();
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

/// Set the group that the Entity is a part of.
/// An entity's group is used for a lot of things.
/// A lot of entity controllers cause controlled entities to "attack" (in some way)
/// enemy entities. By giving each entity a group, and a list of enemy groups,
/// entity controllers can easily figure out which entities should attack which
/// other entities. Similarly, certain entity controllers may want the controlled
/// entity to "heal" or "support" nearby friendly entities.
void Entity::setGroup(int groupNumber)
{
    groupNumber_ = groupNumber;
}

/// Returns the group of the Entity. See setGroup() for more information on some of
/// the kind of things groups are used for.
int Entity::group()
{
    return groupNumber_;
}

/// Adds a group number that this entity should consider enemy.
void Entity::addEnemyGroup(int groupNumber)
{
    enemyGroups_.insert(groupNumber);
}

/// Returns the group numbers that this entity considers enemies.
std::unordered_set<int> Entity::enemyGroups()
{
    return enemyGroups_;
}

/// Returns true if the specified group number is an enemy.
bool Entity::isAnEnemyGroup(int groupNumber)
{
    return enemyGroups_.find(groupNumber) != enemyGroups_.end();
}

/// Add the specified Slot to the Entity.
void Entity::addSlot(Slot *slot)
{
    stringToSlot_[slot->name()] = slot;
    slot->owner_ = this;
}

/// Returns the Slot with the specified name.
/// Returns null if no such slot exists.
Slot *Entity::slot(std::string name)
{
    if (stringToSlot_.count(name) == 0){
        return nullptr;
    }

    return stringToSlot_[name];
}

/// Returns all the slots the Entity has.
std::unordered_set<Slot *> Entity::getSlots()
{
    std::unordered_set<Slot*> allSlots;
    for (std::pair<std::string,Slot*> stringSlotPair:stringToSlot_){
        allSlots.insert(stringSlotPair.second);
    }
    return allSlots;
}


/// Attempts to equip the specified item at the specified Slot.
/// Can only equip Items that are in the Entity's Inventory.
/// If the slot does not accept an item of the specified type, it will
/// not be equipped and this function will return false;
bool Entity::equipItem(EquipableItem *item, std::string slot)
{
    // make sure Entity has the specified Slot
    Slot* slotToEquipIn = this->slot(slot);
    assert(slotToEquipIn != nullptr);

    // equip the Item into the Slot
    bool equipped = slotToEquipIn->equip(item);
    return equipped;
}

/// Overload of Entity::equipItem(EquipableItem*,std::string).
bool Entity::equipItem(EquipableItem *item, Slot *slot)
{
    return equipItem(item,slot->name());
}

/// Sets the Inventory of the Entity to the specified inventory.
void Entity::setInventory(Inventory *inv)
{
    inventory_ = inv;
}

/// Returns the inventory of the Entity.
Inventory *Entity::inventory()
{
    return inventory_;
}

/// Sets the width and height of the Entity's sprite based on the z value of the Entity.
/// This makes it so that Entities higher up are shown bigger.
void Entity::scaleSprite_()
{
    assert(sprite_ != nullptr);

    double scaleFactor = 1.0 + zPos_ / 100.0; // scale up by z%
    int newWidth = sprite_->size().width() * (scaleFactor);
    int newHeight = sprite_->size().height() * (scaleFactor);
    sprite_->setSize(newWidth,newHeight);
}
