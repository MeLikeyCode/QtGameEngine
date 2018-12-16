#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QTimer;

namespace qge{

/// An entity controller (TODO: add doc link) that allows you to rotate
/// an entity. You can have an entity rotate until it faces a certain angle
/// or until it faces a certain point. You can also have an entity rotate to the
/// right or left a certain number of degrees. You can always call stopRotating()
/// to force the entity to stop rotating.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECRotater* c = new ECRotater(entity);
/// c.rotateTowards(someAngle);
/// c.rotateTowards(somePoint);
/// c.rotateLeft(100); // rotate left by 100 degrees
/// c.stopRotating(); // stops the entity from rotating
/// c.isRotating(); // find out if the ECRotater is currently rotating the entity
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// After calling the rotateTowards() method, the entity will begin to rotate
/// towards the specified angle/point. The rate at which the entity rotates
/// can be altered via setRotationSpeed() // TODO <--.
class ECRotater : public EntityController // inherits from QObject so that we can use the 'parent manages lifetime of child' functionality for fields that are QObjects
{
    Q_OBJECT
public:
    ECRotater(Entity* entity);

    // actions
    void rotateTowards(int angle);
    void rotateTowards(const QPointF& point);
    void rotateLeft(int numDegrees);
    void rotateRight(int numDegrees);
    void stopRotating();

    // getter
    bool isRotating() const;

    // setters (options)
    void setStepSize(double degrees);
    double stepSize();

public slots:
    void rotateStep_();
private:
    double stepSize_;

    QTimer* rotationTimer_;

    bool rotateRight_; // true is right, false is left
    int targetAngle_;
    void rotateTowardsTargetAngle_();
};

}
