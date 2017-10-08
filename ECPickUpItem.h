#ifndef ECPICKUPITEM_H
#define ECPICKUPITEM_H

#include <QPointer>

#include "EntityController.h"
#include "Entity.h"

/// An entity controller that causes the controlled entity to pick up items
/// as he collides with them.
class ECPickUpItem: public EntityController
{
    Q_OBJECT
public:
    ECPickUpItem(Entity* entity);

public slots:
    void onCollide(Entity* controlledEntity, Entity* collidedWith);
};

#endif // ECPICKUPITEM_H
