#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

namespace qge{

/// An entity controller (TODO: link to doc) that moves an Entity relative to
/// his own facing angle in response to keyboard keys.
///
/// Example usage:
/// ECMoveByKeyboardPerspective* c = new ECMoveByKeyboardPerspective(entity);
///
/// That is all needed. Once constructed, te Entity passed into the constructor will move
/// in response to the keyboard. WASD keys are used to control the Entity.
/// A and D are used to straf left/right while W and S are used to move forward/backward
/// at current angle.
class ECKeyboardMoverPerspective: public EntityController
{
    Q_OBJECT
public:
    ECKeyboardMoverPerspective(Entity* entity);

    void setStepSize(double stepSize);
    double stepSize();
public slots:
    void moveStep_();
private:
    double stepSize_;

    QTimer* moveTimer_;

    void playAnimIfExistsAndNoOtherPlaying_(std::string anim);
};

}
