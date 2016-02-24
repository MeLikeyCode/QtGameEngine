#ifndef STRAIGHTPROJECTILE_H
#define STRAIGHTPROJECTILE_H

#include "Projectile.h"

/// Represents a projectile that goes straight at its current angle.
/// @see Projectile
class StraightProjectile : public Projectile
{
    Q_OBJECT
public:
    StraightProjectile();

    void setStepSize(int size);
    int stepSize();

    void moveStep();
    void collidedWith(std::vector<Entity*> entities);
    void targetReached();

private:
    int stepSize_;
};

#endif // STRAIGHTPROJECTILE_H
