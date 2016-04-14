#ifndef MOVERELATIVETOSELF_H
#define MOVERELATIVETOSELF_H

// inherits:
#include "PlayerControlledMoveBehavior.h"

// knows about:
class DynamicEntity;

class MoveRelativeToSelf: public PlayerControlledMoveBehavior{
public:
    void setEntity(DynamicEntity* entity);
    void moveStep();
private:
    DynamicEntity* entity_;
};

#endif // MOVERELATIVETOSELF_H
