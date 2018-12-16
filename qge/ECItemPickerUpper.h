#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

namespace qge{

/// An entity controller that causes the controlled entity to pick up items
/// as he collides with them.
class ECItemPickerUpper: public EntityController
{
    Q_OBJECT
public:
    ECItemPickerUpper(Entity* entity);

public slots:
    void onCollide(Entity* controlledEntity, Entity* collidedWith);
};

}
