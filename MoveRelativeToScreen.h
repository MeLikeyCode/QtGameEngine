#ifndef MOVERELATIVETOSCREEN_H
#define MOVERELATIVETOSCREEN_H

// inherits:
#include "PlayerControlledMoveBehavior.h"

// knows of:
class DynamicEntity;

class MoveRealtiveToScreen: public PlayerControlledMoveBehavior{
public:
    void setEntity(DynamicEntity* entity);
    void moveStep();
private:
    DynamicEntity* entity_;
};

#endif // MOVERELATIVETOSCREEN_H
