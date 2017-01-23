#ifndef MOVEBEHAVIOR_H
#define MOVEBEHAVIOR_H

#include <QPointer>
#include "Entity.h"

class QPointF;

/// A behavior object that can move an Entity from its current position to a specified position.
///
/// Example usage
/// =============
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// MoveBehavior* mb = new MoveBehavior(entityToMove);
/// mb->moveTo(somePoint);
/// bool isMoving = mb->isMoving();
/// mb->stopMoving();
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Lets go over the above line by line.
///
/// `MoveBehavior* mb = new MoveBehavior(entityToMove);`
/// Creates a MoveBehavior that can move the Entity passed in it's ctor.
/// Alternatively you can use setEntity() to set the Entity rather than passing it into the ctor.
///
/// `mb->moveTo(somePoint);`
/// Tells the MoveBehavior to move the Entity to some specified position.
/// After the above call, the Entity should start moving to the specified position in
/// whatever manner the concrete MoveBehavior decides.
///
/// `bool isMoving = mb->isMoving();`
/// Checks to see if the MoveBehavior is currently moving the Entity.
///
/// `mb->stopMoving();`
/// Tells the MoveBehavior to stop moving the Entity.
///
/// You cannot call setEntity() while the MoveBehavior is already busy moving an Entity.
/// If you do so, an assertion error will be thrown. You can use isMoving() to determine
/// if the MoveBehavior is currently moving its Entity.
///
/// How to implement your own concrete MoveBehavior
/// ================================================
/// This is an abstract class, not just a pure interface, thus it both defines an interface and
/// provides some default implementation. The non-pure virtual functions of this class can be
/// over riden in sub classes if you don't want the default implementation. The pure virtual function
/// moveTo() obviously MUST be defined in your sub class, as that is what will determine the
/// "style" at which the Entity moves. The protected pure virtual function onStopMoving_() must
/// obviously also be implemented. When someone calls stopMoving() on a MoveBehavior, it will
/// update the isMoving_ flag and then simply call onStopMoving_(). This pattern is often called
/// "template method pattern". It allows the base class to do some initial work, and then
/// delegate the real work to the derived class. You will see this pattern often in this
/// code base, so get a little familar with it.
class MoveBehavior
{
public:
    MoveBehavior(Entity *entity);

    virtual Entity* entity();
    virtual void setEntity(Entity* entity);

    virtual bool isMoving();
    virtual void stopMoving();

    /// Moves the entity to the specified position. Some concrete MoveBehaviors
    /// may move the entity straight, some may move it in a sine motion, some
    /// may move it while spinning it, etc...
    virtual void moveTo(const QPointF& pos) = 0;

protected:
    /// Executed when the MoveBehavior is asked to stop moving.
    /// Derived classes should
    virtual void onStopMoving_() = 0;

private:
    QPointer<Entity> entity_;
    bool isMoving_;
};

#endif // MOVEBEHAVIOR_H
