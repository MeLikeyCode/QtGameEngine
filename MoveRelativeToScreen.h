#ifndef MOVERELATIVETOSCREEN_H
#define MOVERELATIVETOSCREEN_H

// inherits:
#include "PlayerControlledMoveBehavior.h"

// knows of:
class Entity;

class MoveRealtiveToScreen: public PlayerControlledMoveBehavior{
public:
    void setEntity(Entity* entity);
    void moveStep();
private:
    Entity* entity_;
};

#endif // MOVERELATIVETOSCREEN_H
