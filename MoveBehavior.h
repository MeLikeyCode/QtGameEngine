#ifndef MOVEBEHAVIOR_H
#define MOVEBEHAVIOR_H

#include <QPointer>
#include "Entity.h"

class QPointF;

/// Defines an interface for behavior objects that determine how an Entity moves from its current position
/// to a specified position.
///
/// Example usage:
/// // Create a MoveBehavior that can move the Entity passed in it's ctor.
/// // Alternatively you can use setEntity() to set the Entity rather than passing it into the ctor.
/// MoveBehavior* mb = new MoveBehavior(entityToMove);
///
/// // Tell the MoveBehavior to move the Entity to some specified position.
/// mb->moveTo(somePoint);
///
/// // After the above call, the Entity should start moving to the specified position in
/// // whatever manner the concrete MoveBehavior decides. If at any point you want
/// // The MoveBehavior to stop moving the Entity, call:
/// mb->stopMoving();
///
/// // You can swap out the Entity that should be moved even after calling mb->moveTo(), but
/// // keep in mind that doing this will stop moving the old Entity and start moving the new one.
class MoveBehavior
{
public:
    MoveBehavior(Entity *entity);

    Entity* entity();
    void setEntity(Entity* entity);

    /// Moves the entity to the specified position. Some concrete MoveBehaviors
    /// may move the entity straight, some may move it in a sine motion, some
    /// may move it while spinning it, etc...
    virtual void moveTo(const QPointF& pos) = 0;

    /// Stops moving the entity.
    virtual void stopMoving();

private:
    QPointer<Entity> entity_;
};

#endif // MOVEBEHAVIOR_H
