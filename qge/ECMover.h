#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QPointF;

namespace qge{

/// A EntityController that can somehow move an Entity from its current position to a
/// specified position. *How* the object moves depends on the concrete Mover
/// class.
///
/// Example usage
/// =============
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Mover* mover = new SomeConcreteMover(entityToMove);
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
/// @warning
/// You cannot call setEntity() while the Mover is already busy moving an
/// Entity. If you do so, an assertion will be thrown. You can use
/// isMovingEntity() to determine if the Mover is currently moving its Entity.
///
/// How to implement your own concrete Mover class
/// ==============================================
/// 1. Implement the pure virtual function moveEntity_(). This function should
/// start moving the Entity in some way.
///
/// 2. When you have determined that the entity has succesfully been moved,
/// emit the entitySuccesfullyMoved() signal.
///
/// 3. Implement the pure virtual function stopMovingEntity_(). This function
/// should stop moving the Entity.
///
/// See some of the provided concrete Movers for examples of how to create your
/// own Movers.
///
/// @author Abdullah Aghazadah
class ECMover: public EntityController
{
    Q_OBJECT
public:
    ECMover(Entity *entity = nullptr);

    virtual Entity* entity();
    virtual void setEntity(Entity* entity);

    virtual void moveEntity(const QPointF& toPos);
    virtual bool isMovingEntity();
    virtual void stopMovingEntity();

public slots:
    void onEntityDied_(Entity *entity);

signals:
    /// Emitted when the Mover has succesfully moved its Entity to the target
    /// position. Note: It is the responsiblity of concrete Movers to emit this
    /// function when they have determined that the entity has been succesfully
    /// moved.
    void entitySuccesfullyMoved(ECMover* byMover);

protected:
    // Executed when the Mover is asked to move the Entity to the specified
    // position. Concrete implementations of this function should somehow start
    // moving the Entity towards the specified position.
    virtual void moveEntity_(const QPointF& toPos) = 0;

    // Executed when the Mover is asked to stop moving the Entity (via stopMovingEntity()).
    // Concrete implementations of this function should actually stop moving the Entity...
    virtual void stopMovingEntity_() = 0;

private:
    QPointer<Entity> entity_;
    bool isMovingEntity_;
};

}
