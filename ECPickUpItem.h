#ifndef ECPICKUPITEM_H
#define ECPICKUPITEM_H

#include <QObject>
#include <QPointer>
#include "Entity.h"

/// An entity controller that causes the controlled entity to pick up items
/// as he collides with them.
class ECPickUpItem: public QObject
{
    Q_OBJECT
public:
    ECPickUpItem(Entity* entity);

public slots:
    void onCollide(Entity* controlledEntity, Entity* collidedWith);

private:
    QPointer<Entity> entity_;
};

#endif // ECPICKUPITEM_H
