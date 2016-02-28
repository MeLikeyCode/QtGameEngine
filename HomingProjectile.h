#ifndef HOMINGPROJECTILE_H
#define HOMINGPROJECTILE_H

#include "Projectile.h"

class HomingProjectile : public Projectile
{
public:
    HomingProjectile(Entity* entityTarget);

    void moveStep();
    void collidedWith(std::vector<Entity*> entities);
    void targetReached();
private:
    Entity* entityTarget_;
    void faceEntity_();
};

#endif // HOMINGPROJECTILE_H
