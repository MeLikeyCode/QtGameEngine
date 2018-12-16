#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QTimer;

namespace qge{

/// An entity controller (TODO: link to documentation) that moves the entity
/// in 4 directions (up, down, left, and right) in response to the keyboard.
///
/// Example usage:
/// ECMoveByKeyboardFourDirectional* c = new ECMoveByKeyboardFourDirectional(entity);
///
/// Thats all thats needed. Once constructed, the Entity passed into the constructor will move
/// in response to the keyboard. The WASD keys are used to move the entity up, left, down, and right
/// relative to the screen.
/// @date 11/21/16
class ECKeyboardMover4Directional: public EntityController
{
    Q_OBJECT
public:
    ECKeyboardMover4Directional(Entity* entity);

    void setStepSize(double stepSize);
    double stepSize();
public slots:
    void moveStep_();
private:
    double stepSize_;

    QTimer* moveTimer_;

    void playAnimationIfItExists_(std::string animation);
};

}
