#ifndef PLAYERCONTROLLEDMOVEBEHAVIOR_H
#define PLAYERCONTROLLEDMOVEBEHAVIOR_H

// know about:
class Entity;

/// Represents any movement behavior that moves the entity in response
/// to mouse/keyboard input.
class PlayerControlledMoveBehavior
{
public:
    virtual void setEntity(Entity* entity) = 0;
    virtual void moveStep() = 0;
};

#endif // PLAYERCONTROLLEDMOVEBEHAVIOR_H
