#ifndef PROJECTILERANGEREACHEDBEHAVIOR_H
#define PROJECTILERANGEREACHEDBEHAVIOR_H

class Projectile;

/// Represents a behavior that defines what a projectile does when it reaches
/// its range (the distance it was supposed to travel).
class ProjectileRangeReachedBehavior
{
    friend class Projectile;
public:
    virtual void onRangeReached() = 0;
protected:
    Projectile* projectile_;
};

#endif // PROJECTILERANGEREACHEDBEHAVIOR_H
