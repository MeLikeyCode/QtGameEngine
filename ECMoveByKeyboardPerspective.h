#ifndef ECMOVEBYKEYBOARDPERSPECTIVE_H
#define ECMOVEBYKEYBOARDPERSPECTIVE_H

#include <QPointer>

#include "EntityController.h"
#include "Entity.h"

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
class ECMoveByKeyboardPerspective: public EntityController
{
    Q_OBJECT
public:
    ECMoveByKeyboardPerspective(Entity* entity);

    void setStepSize(double stepSize);
    double stepSize();
public slots:
    void moveStep_();
private:
    double stepSize_;

    QTimer* moveTimer_;

    void playAnimIfExists_(std::string anim);
};

#endif // ECMOVEBYKEYBOARDPERSPECTIVE_H
