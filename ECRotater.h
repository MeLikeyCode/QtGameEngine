#ifndef ECROTATE_H
#define ECROTATE_H

#include <QObject>
#include <QPointF>
#include <QPointer>
#include "Entity.h"

class QTimer;

/// An entity controller (TODO: add doc link) that allows you to rotate
/// an entity. You can have an entity rotate until it faces a certain angle
/// or until it faces a certain point. You can also have an entity rotate to the
/// right or left a certain number of degrees. You can always call stopRotating()
/// to force the entity to stop rotating.
///
/// Example usage:
/// ECRotater* c = new ECRotater(entity);
/// c.setRotationSpeed(someSpeed);
/// c.rotateTowards(someAngle);
/// c.rotateTowards(somePoint);
/// c.rotateLeft(100); // rotate left by 100 degrees
/// c.stopRotating(); // stops the entity from rotating
///
/// After calling the rotateTowards() method, the entity will begin to rotate
/// towards the specified angle/point. The rate at which the entity rotates
/// can be altered via setRotationSpeed() // TODO <--.
class ECRotater : public QObject
{
    Q_OBJECT
public:
    ECRotater(Entity* entity);

    void rotateTowards(int angle);
    void rotateTowards(const QPointF& point);
    void rotateLeft(int numDegrees);
    void rotateRight(int numDegrees);
    void stopRotating();
    void setRotationSpeed(double degreesPerSecond);
    double rotationSpeed();

public slots:
    void rotateStep_();
private:
    QPointer<Entity> entity_;
    double rotationFrequency_;

    QTimer* rotationTimer_;

    bool rotateRight_; // true is right, false is left
    int targetAngle_;
    void rotateTowardsTargetAngle_();
    void rotate1Right_();
    void rotate1Left_();

};

#endif // ECROTATE_H
