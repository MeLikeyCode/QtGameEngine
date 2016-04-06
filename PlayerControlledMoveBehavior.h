#ifndef PLAYERCONTROLLEDMOVEBEHAVIOR_H
#define PLAYERCONTROLLEDMOVEBEHAVIOR_H

class DynamicEntity;

/// Represents any movement behavior that moves the entity in response
/// to mouse/keyboard input.
class PlayerControlledMoveBehavior
{
public:
    virtual ~PlayerControlledMoveBehavior() {} // virtual empty desctructor
    virtual void setEntity(DynamicEntity* entity) = 0;
    virtual void moveStep() = 0;
};

#endif // PLAYERCONTROLLEDMOVEBEHAVIOR_H
