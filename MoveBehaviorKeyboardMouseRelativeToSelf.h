#ifndef MOVEBEHAVIORKEYBOARDMOUSERELATIVETOSELF_H
#define MOVEBEHAVIORKEYBOARDMOUSERELATIVETOSELF_H

#include "MoveBehavior.h"

class DynamicEntity;

/// Represents a movement behavior that will move the DynamicEntity in response
/// to the keyboard/mouse (DynamicEntity will face mouse). The Entity moves
/// relative to itself, so going forward will make it move forward at its
/// current angle, going left will make it go to its left, etc...
class MoveBehaviorKeyboardMouseRelativeToSelf : public MoveBehavior
{
    Q_OBJECT
public:
    MoveBehaviorKeyboardMouseRelativeToSelf(DynamicEntity* entity);
public slots:
    virtual void moveStep();
};

#endif // MOVEBEHAVIORKEYBOARDMOUSERELATIVETOSELF_H
