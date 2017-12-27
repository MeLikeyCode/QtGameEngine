#ifndef ENTITY_H
#define ENTITY_H

#include "PathingMap.h"
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <typeinfo> // run time type identification (RTTI)
#include <typeindex>// same ^
#include <set>
#include <QPointer>
#include <utility>
#include <memory>

class Map;
class Inventory;
class EquipableItem;
class Item;
class EntitySprite;
class QPointF;
class Slot;
class Inventory;
class AsyncShortestPathFinder;
class QTimer;
class EntityController;
class PositionalSound;

/// An Entity is the base class for anything that can go inside a Map.
/// @author Abdullah Aghazadah
/// @date 5-20-15
///
/// When an Entity is first added to a Map, the map will use the Entitie's
/// PathingMap and position to update its own PathingMap. When an Entity is
/// moved, it will notify the Map so that it can once again update the
/// PathingMap.
///
/// Entities support parent/child relationships. When a parent Entity moves or
/// rotates, so do all of its children. When a parent Entity is deleted, so
/// are all of its children. When a parent Entity is added/removed to/from a Map,
/// so are all of its children.
///
/// Entities can be controlled by entity controllers (TODO: link to doc).
class Entity: public QObject
{
    Q_OBJECT
    friend class Map; // Map needs to be able to set the map_
                      // pointer of the Entity (we cannot use a public setter
                      // because we don't want ANYONE else to be able to set this)
public:
    // constructor
    Entity();

    // destructor
    virtual ~Entity();

    // pathing map
    PathingMap& pathingMap() const;
    void setPathingMap(PathingMap& pathingMap, const QPointF &pos=QPointF(0,0));
    void setPathingMapPos(const QPointF& to);

    // map
    Map* map() const;
    QPointF mapToMap(const QPointF& point) const;
    QPolygonF mapToMap(const QRectF& rect) const;

    // pos/height/facing angle
    QPointF pos() const;
    double x() const;
    double y() const;
    double z() const;
    double height() const;
    QPointF topLeft() const;
    void setPos(const QPointF &pos);
    void setPos(std::string namedPos, const QPointF& pos);
    void setPos(const QPointF& moveThisPt, const QPointF& toThisPoint);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void moveBy(double dx, double dy);
    void setHeight(double height);
    Node cellPos();
    void setCellPos(const Node& cell);
    QPointF pathingMapPos() const;
    int facingAngle();
    void setFacingAngle(double angle);
    void setZValue(double zValue);
    double zValue();
    QPointF origin() const;
    void setOrigin(const QPointF& to);

    // sprite
    void setSprite(EntitySprite* sprite);
    EntitySprite* sprite() const;

    // parent/child relationship
    std::unordered_set<Entity*> children();
    void setParentEntity(Entity* parent);
    Entity* parent();

    void addNamedPoint(const QPointF& point, std::string name);
    QPointF namedPoint(std::string name);

    void setSpeed(double speed);
    double speed();
    void setRotationSpeed(double rotationSpeed);
    double rotationSpeed();

    void setHealth(double health);
    double health();
    void setMaxHealth(double maxHealth);
    double maxHealth();
    void damage(Entity* entity, double amount);

    void setCanOnlyBeDamagedBy(bool tf);
    void addCanBeDamagedBy(std::type_index typeOfEntity);
    void addCannotBeDamagedBy(std::type_index typeOfEntity);
    bool canBeDamagedBy(Entity* entity);
    bool isInvulnerable();
    void setInvulnerable(bool tf);

    // group/enemy groups
    void setGroup(int groupNumber_);
    int group();
    void addEnemyGroup(int groupNumber);
    std::unordered_set<int> enemyGroups();
    bool isAnEnemyGroup(int groupNumber);

    // slot/equipment/inventory
    void addSlot(Slot* slot);
    Slot* slot(std::string name);
    std::unordered_set<Slot*> getSlots();
    bool equipItem(EquipableItem* item, std::string slot);
    bool equipItem(EquipableItem* item, Slot* slot);
    void setInventory(Inventory* inv);
    Inventory* inventory();

    // misc
    bool canFit(const QPointF& atPos);
    QRectF boundingRect();

    // sounds
    void addSound(const std::string& soundName, const std::string& filepath);
    void playSound(const std::string& soundName, int numTimesToPlay);

public slots:
    void onAnimationFinishedCompletely_(EntitySprite* sender, std::string animation);

signals:
    /// Emitted whenever the Entity moves from a certain position to another
    /// position.
    void moved(Entity* sender, QPointF fromPos, QPointF toPos);

    /// Emitted whenever the Entity collides with another Entity.
    void collided(Entity* sender, Entity* collidedWith);

    /// Emitted whenever the Entity enters a Map.
    /// If the Entity was in another map prior to entering this new map,
    /// "oldMap" parameter will point to that map, otherwise it will be nullptr.
    void mapEntered(Entity* sender, Map* mapJustEntered, Map* oldMap);

    /// Emitted when the entity leaves a Map.
    void mapLeft(Entity* sender, Map* mapJustLeft);

    /// Emitted when the health of the entity changes.
    void healthChanged(Entity* sender);

    /// Emitted right before the entity is about to diee.
    /// Gives listeners one last chance to clear any references to this entity.
    void dying(Entity* sender);

private:
    // main attributes
    PathingMap* pathingMap_;
    QPointF pathingMapPos_;
    QPointF currentPos_;
    QPointF origin_; // location of the sprite that is the considered the "origin" of this entity
    double zPos_;
    double height_;
    double facingAngle_;
    double speed_; // pixels per second
    double rotationSpeed_; // degrees per second
    double zValue_;
    Map* map_;
    EntitySprite* sprite_;
    std::unordered_set<Entity*> children_;
    Entity* parent_;
    std::map<std::string,QPointF> namedPoints_;

    double health_;
    double maxHealth_;
    std::set<std::type_index> canOnlyBeDamagedBy_;
    std::set<std::type_index> canBeDamagedByAllExcept_;
    bool canOnlyBeDamagedByMode_;
    int groupNumber_;
    std::unordered_set<int> enemyGroups_;
    bool invulnerable_;

    Inventory* inventory_;
    std::unordered_map<std::string,Slot*> stringToSlot_;

    QPointF lastPos_;

    std::unordered_map<std::string,std::string> soundNameToFilepath_;
    std::unordered_map<std::string,PositionalSound*> soundPathToPS_;

    // helper functions
    void scaleBasedOnZ_();
};

// make QPointer<Entity> hashable (simply "delegate" to hasing a regular Entity*)
namespace std {
template <> struct hash<QPointer<Entity>>
{
    size_t operator()(const QPointer<Entity>& node) const{
        return hash<Entity*>()(node.data());
    }
};
}

// hash pair of entities (order shouldn't matter)
namespace std{
    template <> struct hash<std::pair<Entity*,Entity*>>{
    size_t operator()(const std::pair<Entity*,Entity*>& pairOfEntities) const{
        return (53 + hash<Entity*>()(pairOfEntities.first)) * 53 + hash<Entity*>()(pairOfEntities.second);
    }
};
}

#endif // ENTITY_H
