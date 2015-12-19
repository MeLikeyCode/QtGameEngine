#ifndef ENTITY_H
#define ENTITY_H

// knows of:
class Map;

// members:
#include "PathingMap.h"
#include "Sprite.h"
#include <QPointF>
#include <unordered_map>
#include "PlayerControlledMoveBehavior.h"

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

    // deconstructor
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
    Node cellPos();
    void setCellPos(const Node& cell);

    void setSprite(Sprite* sprite);
    Sprite* sprite() const;
    QRectF boundingRect();

    int facingAngle();
    void setFacingAngle(double angle);

    std::vector<Entity*> children() const;
    void setParentEntity(Entity* parent);

    QPointF mapToMap(const QPointF& point) const;

    void addNamedPoint(const QPointF& point, std::string name);
    QPointF namedPoint(std::string name);

private:
    // main attributes
    PathingMap pathingMap_;
    Map* map_;
    Sprite* sprite_;
    std::vector<Entity*> children_;
    Entity* parent_;
    std::map<std::string,QPointF> namedPoints_;
};

#endif // ENTITY_H
