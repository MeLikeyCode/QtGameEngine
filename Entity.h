#ifndef ENTITY_H
#define ENTITY_H

#include "PathingMap.h"
#include "Sprite.h"
#include <QPointF>
#include <unordered_map>
#include "PlayerControlledMoveBehavior.h"
#include <string>

class Map;
class Inventory;
class EquipableItem;
class Item;


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
/// are all of its children.
class Entity
{
public:
    // constructor
    Entity();

    // destructor
    virtual ~Entity();

    PathingMap pathingMap() const;
    void setPathingMap(const PathingMap& pathingMap);
    bool canFit(const QPointF& atPos);
    void disablePathingMap();
    void enablePathingMap();

    Map* map() const;
    void setMap(Map* toMap);

    QPointF pointPos() const;
    void setPointPos(const QPointF &pos);
    void setPointPos(std::string namedPos, const QPointF& pos);
    void setPointPos(const QPointF& moveThisPt, const QPointF& toThisPoint);
    Node cellPos();
    void setCellPos(const Node& cell);

    void setSprite(Sprite* sprite);
    Sprite* sprite() const;
    QRectF boundingRect();

    int facingAngle();
    void setFacingAngle(double angle);

    std::vector<Entity*> children() const;
    void setParentEntity(Entity* parent);
    Entity* parent();

    QPointF mapToMap(const QPointF& point) const;

    void addNamedPoint(const QPointF& point, std::string name);
    QPointF namedPoint(std::string name);

    void equipItem(EquipableItem* item, const QPointF& at);
    void addItemToInventory(Item* item);

private:
    // main attributes
    PathingMap pathingMap_;
    Map* map_;
    Sprite* sprite_;
    std::vector<Entity*> children_;
    Entity* parent_;
    std::map<std::string,QPointF> namedPoints_;
    Inventory* inventory_;
};

#endif // ENTITY_H
