#ifndef ECMOVEBYKEYBOARDFOURDIRECTIONAL_H
#define ECMOVEBYKEYBOARDFOURDIRECTIONAL_H


#include <QObject>
#include <QPointer>
#include "Entity.h"

class QTimer;

/// An entity controller (TODO: link to documentation) that moves the entity
/// in 4 directions (up, down, left, and right) in response to the keyboard.
///
/// Example usage:
/// ECMoveByKeyboardFourDirectional* c = new ECMoveByKeyboardFourDirectional(entity);
///
/// That is all that is needed. Once constructed, the Entity passed into the constructor will move
/// in response to the keyboard. The WASD keys are used to move the entity up, left, down, and right
/// relative to the screen.
/// @date 11/21/16
class ECMoveByKeyboardFourDirectional: public QObject
{
    Q_OBJECT
public:
    ECMoveByKeyboardFourDirectional(Entity* entity);

    void setStepSize(double stepSize);
    double stepSize();
public slots:
    void moveStep_();
private:
    QPointer<Entity> entity_;   // TODO factor this attribute out into base EntityBehavior class
                                // this is a QPointer because we wanna know when the Entity is deleted
                                // so that we can stop moving it (dangling pointer!)
    double stepSize_;

    QTimer* moveTimer_;
};
#endif // ECMOVEBYKEYBOARDFOURDIRECTIONAL_H
