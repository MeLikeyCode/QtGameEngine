#ifndef PROJECTILEMOVEBEHAVIOR_H
#define PROJECTILEMOVEBEHAVIOR_H

class Projectile;

/// Represents a behavior that defines how a Projectile should move every time
/// it needs to move.
class ProjectileMoveBehavior
{
    friend class Projectile;
public:
    virtual double onMoveStep() = 0;
protected:
    Projectile* projectile_;
};

#endif // PROJECTILEMOVEBEHAVIOR_H
