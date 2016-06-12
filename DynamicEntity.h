#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

#include "Entity.h"
#include "PathingMap.h"
#include "Sprite.h"
#include <QPointF>
#include <unordered_map>
#include "PlayerControlledMoveBehavior.h"
#include "AsyncShortestPathFinder.h"
#include "unordered_set"

class MeleeWeapon; // TODO remove, test only
class Map;
class Weapon;
class Slot;

/// An Entity that can move around, see, and take action.
/// @author Abdullah Aghazadah
/// @date 5-20-15
///
/// A DynamicEntity is an Entity that has functionality for moving from point to
/// point and rotating in real time.
class DynamicEntity: public Entity{
    Q_OBJECT
public:
    // constructor
    DynamicEntity();

    // deconstructor
    virtual ~DynamicEntity();

    void setStepSize(int stepSize);
    int stepSize();
    void setStepFrequency(int to);
    int stepFrequency();

    int rotationFrequency() const;
    void setRotationFrequency(int freq);
    void rotateRight();
    void rotateLeft();
    void rotateRight(int degrees);
    void rotateLeft(int degrees);
    void rotateTo(int angle);
    void rotateTo(QPointF point);
    void stopRotating();

    void moveTo(QPointF pos);
    void moveTo(const Node& cell);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopAutomaticMovement();
    void setPlayerControlledMoveBehavior(PlayerControlledMoveBehavior* behavior);
    bool isMoving();

    bool isPlayerControlled();
    void setPlayerControlled(bool tf);

    std::unordered_set<Entity*> entitiesInView();

    void addSlot(Slot* slot);
    Slot* slot(std::string name);
    std::unordered_set<Slot*> getSlots();
    void equipItem(EquipableItem* item, std::string slot);
    void equipItem(EquipableItem* item, Slot* slot);
    void addItemToInventory(Item* item);
    void removeItemFromInventory(Item* item);
    bool inventoryContains(Item* item);
    Inventory* inventory();

signals:
    void collided(std::unordered_set<Entity*>);

public slots:
    void moveStepPlayerControlled();
    void moveStepAIControlled_();
    void rotateStep();
    void followPath_(std::vector<QPointF> path);
    void onCollided(std::unordered_set<Entity*> entities);

private:
    // main attributes
    int rotationFrequency_;

    // moving
    int stepSize_;
    int stepFrequency_;
    QTimer* moveTimer_;
    bool targetPointReached();
    void stepTowardsTarget();
    std::vector<QPointF> pointsToFollow_;
    int targetPointIndex_;
    int targetAngle_;
    bool rotateRight_; // true is right, false is left
    QTimer* rotationTimer_;
    void rotateTowardsTargetAngle();
    bool isPlayerControlled_;
    PlayerControlledMoveBehavior* moveBehavior_;
    double timeStuck_;

    // field of view
    double fieldOfViewDistance_;
    double fieldOfViewAngle_; // FULL angle

    // inventory/items
    Inventory* inventory_;
    std::unordered_map<std::string,Slot*> stringToSlot_;

    AsyncShortestPathFinder* pf_;
};

#endif // DYNAMICENTITY_H
