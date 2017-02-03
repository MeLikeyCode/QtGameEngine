#ifndef MOVER_H
#define MOVER_H

#include <QPointer>
#include "Entity.h"

class QPointF;

/// An object that can move an Entity from its current position to a specified
/// position.
///
/// Example usage
/// =============
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Mover* mover = new Mover(entityToMove);
/// mover->moveEntity(toSomePosition);
/// bool isMoving = mover->isMovingEntity();
/// mover->stopMovingEntity();
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Lets go over the above line by line.
///
/// `Mover* mover = new Mover(entityToMove);`
/// Creates a Mover that can move the Entity passed in it's ctor. Alternatively
/// you can use setEntity() to set the Entity rather than passing it into the
/// ctor.
///
/// `mover->moveEntity(toSomePosition);`
/// Tells the Mover to move the Entity to some specified position. After the
/// above call, the Entity should start moving to the specified position in
/// whatever manner the concrete Mover class decides.
///
/// `bool isMoving = mover->isMovingEntity();`
/// Checks to see if the Mover is currently moving the Entity.
///
/// `mover->stopMovingEntity();`
/// Tells the Mover to stop moving the Entity.
///
/// Note
/// ====
///
/// You cannot call setEntity() while the Mover is already busy moving an
/// Entity. If you do so, an assertion will be thrown. You can use
/// isMovingEntity() to determine if the Mover is currently moving its Entity.
///
/// How to implement your own concrete Mover class
/// ==============================================
/// This is an abstract class, not just a pure interface, thus it both defines
/// an interface and provides some default implementations. But all the default
/// implementations can be overrided if they don't suite you (in other words,
/// they are non-pure virtual functions).
///
/// The pure virtual function moveEntity() obviously MUST be implemented in
/// your sub class, as that is the function that is supposed to do the actual
/// moving of the entity. I suggest you use a QTimer to periodically move the
/// Entity in the manner of your choosing.
///
/// The protected pure virtual function stopMovingEntity_() must obviously also
/// be implemented. When someone calls stopMovingEntity_() on a Mover, it will
/// update the isMovingEntity_ flag and then simply call stopMovingEntity_().
/// This pattern is often called "template method pattern". It allows the base
/// class to do some initial work, and then delegate the real work to the
/// derived class. You will see this pattern often in this code base, so get a
/// little familar with it.
///
/// @author Abdullah Aghazadah
class Mover
{
public:
    Mover(Entity *entity);

    virtual Entity* entity();
    virtual void setEntity(Entity* entity);

    virtual bool isMovingEntity();
    virtual void stopMovingEntity();

    /// Moves the entity to the specified position. Some concrete Movers
    /// may move the entity straight, some may move it in a sine motion, some
    /// may move it while spinning it, etc...
    virtual void moveEntity(const QPointF& toPos) = 0;

protected:
    // Executed when the Mover is asked to stop moving the Entity (via stopMovingEntity()).
    // Concrete implementations of this function should actually stop moving the Entity...
    virtual void stopMovingEntity_() = 0;

private:
    QPointer<Entity> entity_;
    bool isMovingEntity_;
};

#endif // MOVER_H
