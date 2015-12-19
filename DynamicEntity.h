#ifndef DYNAMICENTITY_H
#define DYNAMICENTITY_H

class Spear; // TODO remove, test only

// uses:
class Map;

// inherits:
#include "Entity.h"

// members:
#include "PathingMap.h"
#include "Sprite.h"
#include <QPointF>
#include <unordered_map>
#include "PlayerControlledMoveBehavior.h"

/// An Entity that can move around on its own or in resposne to user input.
/// @author Abdullah Aghazadah
/// @date 5-20-15
///
/// A DynamicEntity is an Entity that has functionality for moving from point to
/// point and rotating in real time.
class DynamicEntity:public QObject, public Entity{
    Q_OBJECT
public:
    // constructor
    DynamicEntity();

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

    void moveTo(const QPointF& pos);
    void moveTo(const Node& cell);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void stopAutomaticMovement();
    void setPlayerControlledMoveBehavior(PlayerControlledMoveBehavior* behavior);


    bool isPlayerControlled();
    void setPlayerControlled(bool tf);

    // TODO for testing out spear only, delete after
    Spear* spear_;
    Spear* spear2_;

public slots:
    void moveStepPlayerControlled();
    void moveStepAIControlled();
    void rotateStep();

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


};

#endif // DYNAMICENTITY_H
