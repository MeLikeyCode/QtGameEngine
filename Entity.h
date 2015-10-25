#ifndef ENTITY_H
#define ENTITY_H

class Spear; // TODO remove, test only

// uses:
class Map;

// inherits:
#include <QGraphicsItem>

// members:
#include "PathingMap.h"
#include "Sprite.h"
#include <QPointF>
#include <unordered_map>

/// An Entity is the base class for anything that can go inside a Map.
/// @author Abdullah Aghazadah
/// @date 5-20-15
///
/// When an Entity is first added to a Map, the map will use the Entitie's
/// PathingMap and position to update its own PathingMap. When an Entity is
/// moved, it will notify the Map so that it can once again update the
/// PathingMap.
///
/// An entity also has health and energy and provides functions for moving around
/// the map. Furthermore, an entity has "sight" in the sense that it provdes
/// functions for retrieving all other entities within a certain radius of it
/// (this is delgated to the map).
class Entity:public QObject{
    Q_OBJECT
public:
    // constructor
    Entity();

    // deconstructor
    ~Entity();

    PathingMap pathingMap() const;
    void setPathingMap(const PathingMap& pathingMap);

    Map* map() const;
    void setMap(Map* toMap);

    QPointF pointPos() const;
    void setPointPos(const QPointF &pos);
    Node cellPos();
    void setCellPos(const Node& cell);

    void setSprite(Sprite* sprite);
    Sprite* sprite() const;
    QRectF boundingRect();

    void setStepSize(int stepSize);
    int stepSize();
    void setStepFrequency(int to);
    int stepFrequency();

    int rotationFrequency() const;
    void setRotationFrequency(int freq);
    int facingAngle();
    void rotateRight();
    void rotateLeft();
    void rotateRight(int degrees);
    void rotateLeft(int degrees);
    void rotateTo(int angle);
    void rotateTo(QPointF point);
    void stopRotating();

    void moveTo(const QPointF& pos);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopAutomaticMovement();

    bool canMovesWithKeys();
    void setCanMoveWithKeys(bool b);
    bool canFit(const QPointF& atPos);

    void disablePathingMap();
    void enablePathingMap();

    std::vector<Entity*> children() const;
    void setParentEntity(Entity* parent);

    QPointF mapToMap(const QPointF& point) const;

    void addNamedPoint(const QPointF& point, std::string name);
    QPointF namedPoint(std::string name);

    // TODO for testing out spear only, delete after
    Spear* spear_;
    Spear* spear2_;

public slots:
    void moveStepWithKeys();
    void moveStepAutomatic();
    void rotateStep();

private:
    // main attributes
    PathingMap pathingMap_;
    Map* map_;
    Sprite* sprite_;
    int rotationFrequency_;
    std::vector<Entity*> children_;
    Entity* parent_;


    // moving
    int stepSize_;
    int stepFrequency_;
    QTimer* automaticMoveTimer_;
    bool targetPointReached();
    void stepTowardsTarget();
    std::vector<QPointF> pointsToFollow_;
    int targetPointIndex_;
    int targetAngle_;
    bool rotateRight_; // true is right, false is left
    QTimer* rotationTimer_;
    void rotateTowardsTargetAngle();
    bool canMoveWithKeys_;
    QTimer* moveWithKeysTimer_;
    bool movingWithKeys_;
    std::map<std::string,QPointF> namedPoints_;

};

#endif // ENTITY_H
