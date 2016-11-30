#ifndef MOVERELATIVETOSELF_H
#define MOVERELATIVETOSELF_H

// inherits:
#include "PlayerControlledMoveBehavior.h"

// knows about:
class Entity;

class MoveRelativeToSelf: public PlayerControlledMoveBehavior{
public:
    void setEntity(Entity* entity);
    void moveStep();
private:
    Entity* entity_;
};

#endif // MOVERELATIVETOSELF_H
