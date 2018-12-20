#include "Entity.h"

#include "Map.h"
#include "EntitySprite.h"
#include "TopDownSprite.h"
#include "Game.h"
#include "EquipableItem.h"
#include "Inventory.h"
#include "Slot.h"
#include "QGraphicsItem"
#include "EntityController.h"
#include "Utilities.h"
#include "STLWrappers.h"

using namespace qge;

/// Constructs a default entity.
Entity::Entity():
    pathingMap_(new PathingMap(1,1,32)),            // default 1x1 unfilled (in body) PathingMap
    map_(nullptr),
    sprite_(new TopDownSprite()),
    children_(),
    parent_(nullptr),
    health_(10),                    // default health of 10
    maxHealth_(100),                // deafult max health of 100
    groupNumber_(0),                     // default group id of 0
    invulnerable_(false),
    zPos_(0),
    height_(0),
    inventory_(new Inventory()),
    speed_(250),
    facingAngle_(0),
    rotationSpeed_(360)
{
    sprite_->setParent(this);
    inventory_->entity_ = this;
}

/// When an Entity is deleted, it will delete all of its children, and then
/// remove itself from its Map. As each child is deleted, it will
/// delete its own children and then remove itself from the Map. You can kinda
/// see how the "flow" of deletion happens :). The "deepest" child is deleted
/// and removed from the Map first.
Entity::~Entity()
{
    emit dying(this); // let listeners know this entity is *about* to die

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
}

/// Returns (a reference to) the PathingMap of the Entity.
PathingMap& Entity::pathingMap() const{
    return *pathingMap_;
}

/// Sets the PathingMap of the Entity.
/// The old PathingMap is removed from the entity's Map, however it is not deleted.
/// Client is responsible for lifetime of old map.
/// @param pos The pos that the PathingMap is placed relative to the Entity.
void Entity::setPathingMap(PathingMap &pathingMap, const QPointF& pos){
    PathingMap* oldPM = pathingMap_;

    // update internal variables
    pathingMap_ = &pathingMap;
    pathingMapPos_ = pos;

    // remove old pathing map from map
    Map* entitysMap = map();
    if (entitysMap)
        entitysMap->removePathingMap(*oldPM);
}

/// Sets the position of the entity's PathingMap relative to the entity.
void Entity::setPathingMapPos(const QPointF &to)
{
    pathingMapPos_ = to;
}

/// Returns a pointer to the Map that the Entity is in. If the Entity is not
/// in any Maps, returns nullptr.
Map *Entity::map() const{
    return map_;
}

/// Returns the position of the Entity.
///
/// The position is returned relative to the parent Entity. If there is no
/// parent Entitiy, it is returned relative to the Map.
///
/// This position of an entity isn't necessarly the top left or center of its sprite.
/// EntitySprite::origin() tells you which pos of the sprite will count as the pos of the entity.
QPointF Entity::pos() const{
    return currentPos_;
}

/// Returns the position of the Entity's PathingMap relative to the Entity.
QPointF Entity::pathingMapPos() const
{
    return pathingMapPos_;
}

/// Returns the x position of the Entity relative to the parent, relative to
/// Map if no parent.
double Entity::x() const
{
    return pos().x();
}

/// Returns the y position of the Entity relative to the parent, relative to
/// Map if no parent.
double Entity::y() const
{
    return pos().y();
}

/// Returns the z position of the Entity. The z position is the height
/// above level ground that the Entity is at.
double Entity::z() const
{
    return zPos_;
}

/// Returns the height of the Entity.
double Entity::height() const
{
    return height_;
}

/// Returns the top left position of the Entity (its sprite) relative to the parent entity.
/// If the entity has no parent entity, returns the top left relative to the Map.
QPointF Entity::topLeft() const
{
    return currentPos_ - origin(); // TODO this function might be only correct when sprite is unrotated, use mapping to make more accurate
}

/// Returns the bottom right position of the Entity (its sprite) relative to the parent Entity.
/// If the entity has no parent entity, returns the top left relative to the Map.
QPointF Entity::botRight() const
{
    return currentPos_ + origin();
}

/// Sets the position of the Entity.
///
/// The position is relative to the parent Entity. If there is no
/// parent Entitiy, it is relative to the Map.
void Entity::setPos(const QPointF &pos){
    // set internal variable
    currentPos_ = pos;

    // if has a sprite
    EntitySprite* entitysSprite = sprite();
    if (entitysSprite != nullptr){
        entitysSprite->underlyingItem_->setPos(pos - origin()); // set position of sprite
    }

    // if the Entity is in a Map
    Map* entitysMap = map();
    if (entitysMap){
        // update the PathingMap
        entitysMap->removePathingMap(pathingMap());
        entitysMap->addPathingMap(pathingMap(),mapToMap(pathingMapPos()));
        entitysMap->updatePathingMap();

        // update z
        qreal bot = mapToMap(boundingRect().bottomRight()).y();
        sprite()->underlyingItem_->setZValue(bot); // set z value (lower in map -> draw higher on top)

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
    if (lastPos_ != this->pos()){
        QPointF lastPosCpy = lastPos_;
        lastPos_ = this->pos(); // update last position
        emit moved(this,lastPosCpy,this->pos());
    }

}

/// Sets the position of the Entity by moving the specified named position.
void Entity::setPos(std::string namedPos, const QPointF &pos)
{
    // move regularly
    setPos(pos);

    // shift by amount
    QLineF line(namedPoint("namedPos"),QPointF(0,0));
    double newX = this->pos().x() - line.dx();
    double newY = this->pos().y() - line.dy();
    setPos(QPointF(newX,newY));
}

/// Sets the position of the Entity's specific point.
///
/// So if you wanted to move the Entity's 50,50 to somewhere, you would use this
/// function.
void Entity::setPos(const QPointF &moveThisPt, const QPointF &toThisPoint)
{
    // move regularly
    setPos(toThisPoint);

    // shift back
    QLineF line(moveThisPt,QPointF(0,0));
    double newX = pos().x() + line.dx();
    double newY = pos().y() + line.dy();
    setPos(QPointF(newX,newY));
}

/// Sets the x position of the Entity.
void Entity::setX(double x)
{
    QPointF newPoint(x,y());
    setPos(newPoint);
}

/// Sets the y position of the Entity.
void Entity::setY(double y)
{
    QPointF newPoint(x(),y);
    setPos(newPoint);
}

/// Sets the z position of the Entity.
/// See Entity:pointZ() for more info.
void Entity::setZ(double z)
{
    zPos_ = z;

    // scale according to z
    if (sprite_ != nullptr){
        scaleBasedOnZ_();
    }
}

/// Moves the entity by the specified dx,dy.
void Entity::moveBy(double dx, double dy)
{
    setPos(QPointF(x() + dx, y() + dy));
}

/// Sets the height of the Entity.
void Entity::setHeight(double height)
{
    height_ = height;
}

/// Returns the cell that the Entity is in.
Node Entity::cellPos(){
    return map()->pathingMap().pointToCell(pos());
}

/// Sets the Sprite of the Entity.
/// This does *not* delete the old sprite. You are responsible for the old sprite's lifetime. Use
/// sprite() to get a pointer to the old sprite before calling this function.
/// @param autoSetOriginAndBBox If true, will automatically update the origin of the entity to be
/// at the very center of the new sprite, and will automatically update the bounding box of the entity
/// to encompass the whole sprite. For most sprites, you usually want to do this, that is why the
/// parameter defaults to true. But if for example, the sprite has a lot of transparent pixels
/// along the borders, you may want to leave your manually set bounding box.
void Entity::setSprite(EntitySprite *sprite, bool autoSetOriginAndBBox){
    // set all childrens' sprites' parent to new sprite
    for (Entity* child: children()){
        child->sprite()->underlyingItem_->setParentItem(sprite->underlyingItem_);
    }

    // make sure the new sprite is positioned correctly on the scene
    sprite->underlyingItem_->setPos(currentPos_ - origin());

    // if the Entity is already in a map
    if (map_){
        // remove old sprite/add new sprite
        map()->scene()->removeItem(sprite_->underlyingItem_);
        map()->scene()->addItem(sprite->underlyingItem_);

        // ensure sprite has proper z
        qreal bot = mapToMap(boundingRect().bottomRight()).y();
        sprite->underlyingItem_->setZValue(bot); // set z value (lower in map -> draw higher on top)
    }

    // auto set origin and bounding box
    if (autoSetOriginAndBBox){
        double cx = sprite->boundingBox().width() / 2;
        double cy = sprite->boundingBox().height() / 2;
        setOrigin(QPointF(cx,cy));
        boundingRect_ = sprite->boundingBox();
    }

    // set internal sprite_ pointer to the new sprite
    sprite_ = sprite;

    // set scaling of the new sprite
    scaleBasedOnZ_();
}

/// Returns the Entity's Sprite. If the Entity does not have a sprite,
/// returns nullptr.
EntitySprite *Entity::sprite() const{
    return sprite_;
}

/// Returns the bounding rect of the entity.
/// The bounding rect is the area of the entity's sprite that is considered to be it's "solid" part.
/// This is the area that will be used to test for collisions and such.
QRectF Entity::boundingRect() const
{
    return boundingRect_;
}

/// Adds a sound to the entity with the specified name.
void Entity::addSound(const std::string &soundName, const std::string &filepath)
{
    soundNameToFilepath_[soundName] = filepath;
}

/// Plays the specified sound, if it exists, else does nothing.
void Entity::playSound(const std::string &soundName, int numTimesToPlay)
{
    Map* entitysMap = map();
    assert(entitysMap != nullptr);

    // do nothing if specified sound doesn't exists
    auto itrToFilepath = soundNameToFilepath_.find(soundName);
    if (itrToFilepath == std::end(soundNameToFilepath_))
        return;

    std::string filepath = itrToFilepath->second;
    auto itrPS = soundPathToPS_.find(filepath);

    // create positional sound if necessary
    if (itrPS == std::end(soundPathToPS_)){
        soundPathToPS_[filepath] = new PositionalSound(entitysMap,filepath,pos());
        itrPS = soundPathToPS_.find(filepath);
    }

    // play
    PositionalSound* ps = itrPS->second;
    ps->play(numTimesToPlay);

}

void Entity::onAnimationFinishedCompletely_(EntitySprite *sender, std::string animation)
{
    if (animation == "die" || animation == "dieTwo"){
        // self disconnect
        disconnect(sender,&EntitySprite::animationFinishedCompletely,this,&Entity::onAnimationFinishedCompletely_);

        deleteLater();
    }
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
    if (sprite_)
        sprite_->setFacingAngle(angle);
}

/// Makes the Entity face a certain point in the map.
void Entity::facePoint(const QPointF &point)
{
    assert(map());

    QLineF line(pos(),point);
    setFacingAngle(-1 * line.angle());
}

/// Returns the origin of the Entity.
/// @see setOrigin()
QPointF Entity::origin() const
{
    return origin_;
}

/// Sets the point, relative to the top left corner of its sprite, that the entity's "origin" should
/// be considered. When the position of the Entity is set, this is the point of the sprite that is
/// moved to the specified position. When the Entity rotates, this is the point about which the sprite
/// will rotate.
void Entity::setOrigin(const QPointF &to)
{
    origin_ = to;
}

/// Sets the bounding rect of the entity.
/// @see boundingRect()
void Entity::setBoundingRect(const QRectF &rect)
{
    boundingRect_ = rect;
}

/// Instantly moves the Entity to the specified cell in the Map.
void Entity::setCellPos(const Node &cell){
    // make sure the entity has a Map
    assert(map_);

    // move to new region
    QPointF newPos = map()->pathingMap().cellToPoint(cell);
    setPos(newPos);

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

    Map* m = map();

    // temporarly remove own pathing map
    m->removePathingMap(pathingMap());
    m->updatePathingMap();
    bool can = m->pathingMap().canFit(pathingMap(),atPos);
    m->addPathingMap(pathingMap(),mapToMap(pathingMapPos()));
    m->updatePathingMap();

    return can;
}

/// Returns the children of this Entity.
std::unordered_set<Entity *> Entity::children() const
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
        sprite()->underlyingItem_->setParentItem(nullptr);
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
        sprite()->underlyingItem_->setParentItem(parent->sprite()->underlyingItem_);
    }
}

/// Returns the Entity's parent.
Entity *Entity::parent()
{
    return parent_;
}

/// Returns true if `entity` is a direct child of this Entity.
bool Entity::hasChild(Entity* entity) const
{
    return children_.find(entity) != std::end(children_);
}

/// Returns true if `entity` is a child (or a child of a child) of this Entity.
bool Entity::hasChildRecursive(Entity *entity) const
{
    // base case
    if (hasChild(entity))
        return true;

    // recursive case
    for (Entity* child: children_){
        if (child->hasChildRecursive(entity))
            return true;
    }

    return false;
}

/// Maps a point from local (Entity) coordinates to the Map (scene) coordinates.
QPointF Entity::mapToMap(const QPointF &point) const
{
    return sprite()->underlyingItem_->mapToScene(point);
}

QPolygonF Entity::mapToMap(const QRectF &rect) const
{
    return sprite()->underlyingItem_->mapToScene(rect);
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

/// Adds a tag to the Entity.
/// Tags are just a way to associate string information with entities.
void Entity::addTag(const std::string &tag)
{
    STLWrappers::add(tags_,tag);
}

/// Returns true if the Entity has the specified tag.
/// @see addTag()
bool Entity::containsTag(const std::string &tag)
{
    return STLWrappers::contains(tags_,tag);
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

/// Sets the health of the entity.
/// If set below 0, entity dies.
/// If the entity has a a "die" or "dieTwo" animation, one of the two will be played and then the
/// entity will be destroyed.
/// This function will not allow the health to be set above the maximum health (will clamp it),
/// use setMaxHealth() if you want to change the maximum health of an entity.
void Entity::setHealth(double health)
{
    if (isInvulnerable())
        return;

    health_ = health; // update internal var (clamp to max)
    if (health_ > maxHealth())
        health_ = maxHealth();

    emit healthChanged(this);

    if (health_ < 0){ // if health is below z, kill entity

        // if entity has a death anim, play it first, then kill it
        EntitySprite* spr = sprite();
        if (spr){
            if (spr->hasAnimation("die")){
                connect(spr,&EntitySprite::animationFinishedCompletely,this,&Entity::onAnimationFinishedCompletely_);
                int r = randInt(1,2);
                if (r == 2 && spr->hasAnimation("dieTwo")){
                    spr->play("dieTwo",1,10,0);
                    playSound("die",1);
                    return;
                }
                else{
                    spr->play("die",1,10,0);
                    playSound("die",1);
                    return;
                }
            }
        }

        deleteLater();
    }
}

double Entity::health()
{
    return health_;
}

void Entity::setMaxHealth(double maxHealth)
{
    maxHealth_ = maxHealth;
}

double Entity::maxHealth()
{
    return maxHealth_;
}

/// Damages the specified entity, but only if it's an enemy.
void Entity::damageEnemy(Entity *enemy, double amount) const
{
    DiplomacyManager& dm = Game::game->diplomacyManager();
    Relationship relation = dm.getRelationship(group(),enemy->group());
    if (relation == Relationship::ENEMY){
        enemy->setHealth(enemy->health() - amount);
    }

    // play hit animation
    if (enemy->sprite()->hasAnimation("hit"))
        enemy->sprite()->playThenGoBackToOldAnimation("hit",1,10,0);
}

/// Damages the specified entity, but only if it is an enemy or neutral.
void Entity::damageEnemyAndNeutral(Entity *enemyOrNeutral, double amount) const
{
    DiplomacyManager& dm = Game::game->diplomacyManager();
    Relationship relation = dm.getRelationship(group(),enemyOrNeutral->group());
    if (relation == Relationship::ENEMY || relation == Relationship::NEUTRAL){
        enemyOrNeutral->setHealth(enemyOrNeutral->health() - amount);
    }

    // play hit animation
    if (enemyOrNeutral->sprite()->hasAnimation("hit"))
        enemyOrNeutral->sprite()->playThenGoBackToOldAnimation("hit",1,10,0);
}

/// Damages the specified entity, but only if it's not a child of this entity.
void Entity::damageAnyoneExceptChildren(Entity *entity, double amount) const
{
    // if entity isn't a child (or child of child), then damage it
    if (!hasChildRecursive(entity))
        entity->setHealth(entity->health() - amount);

    // play hit animation
    if (entity->sprite()->hasAnimation("hit"))
        entity->sprite()->playThenGoBackToOldAnimation("hit",1,10,0);
}

/// Damages he specified entity, regardless of relationship.
void Entity::damageAnyone(Entity *anyEntity, double amount) const
{
    anyEntity->setHealth(anyEntity->health() - amount);

    // play hit animation
    if (anyEntity->sprite()->hasAnimation("hit"))
        anyEntity->sprite()->playThenGoBackToOldAnimation("hit",1,10,0);
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
/// @see DiplomacyManager
void Entity::setGroup(int groupNumber)
{
    groupNumber_ = groupNumber;
}

/// Returns the group of the Entity. See setGroup() for more information on some of
/// the kind of things groups are used for.
int Entity::group() const
{
    return groupNumber_;
}

/// Tells you how this entity feels towards the specified entity.
/// @see DiplomacyManager
Relationship Entity::relationshipTowards(const Entity &otherEntity) const
{
    return Game::game->diplomacyManager().getRelationship(group(),otherEntity.group());
}

/// Add the specified Slot to the Entity and gives it the specified name.
void Entity::addSlot(Slot *slot, const std::string &name)
{
    slot->setName(name);
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
void Entity::scaleBasedOnZ_()
{
    assert(sprite_ != nullptr);

    double scaleFactor = 1.0 + zPos_ / 100.0; // scale up by z%
    sprite_->scale(scaleFactor);
}
