#ifndef MOVEBEHAVIORKEYBOARDMOUSE_H
#define MOVEBEHAVIORKEYBOARDMOUSE_H

#include "MoveBehavior.h"

class DynamicEntity;

/// Represents a movement behavior that will move the DynamicEntity in
/// response to the keyboard/mouse (DynamicEntity will face mouse). The Entity
/// will move relative to the screen, so going forward will move it up
/// relative to the screen, going left will move it left relative to the screen,
/// etc...
class MoveBehaviorKeyboardMouseRelativeToScreen : public MoveBehavior
{
    Q_OBJECT
public:
    MoveBehaviorKeyboardMouseRelativeToScreen(DynamicEntity* entity);
public slots:
    virtual void moveStep();
};

#endif // MOVEBEHAVIORKEYBOARDMOUSE_H
