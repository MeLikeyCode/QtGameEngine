#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include <QObject>
#include "Entity.h"

/// An object that provides some sort of behavior for an Entity (i.e "controls" the Entity in some way).
/// @warning This class is not meant to be instantiated, it is an abstract class. Construct one of the
/// concrete base classes instead.
///
/// @author Abdullah Aghazadah
/// @date 10/6/17
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Entity* e; // assume e points to some entity
/// EntityController* c = new SomeConcreteEntityController(e);
/// c.setEntity(e); // make the EntityController control Entity e
/// c.entity(); // get the Entity that the EntityController is controlling/operating on
/// // Note: Concrete EntityControllers will most likely have additional methods and/or signals.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// The best way to get an idea of what kind of things EntityControllers do with their controlled entity
/// is to look at a few example EntityControllers.
///
/// @note As a naming convention, all concrete EntityControllers have the prefix EC.
///
/// ECFieldOfViewEmitter will emit a signal whenever another entity either enters or leaves its controlled
/// entity's field of view. ECMoveByKeyboardFourDirectional will move its controlled entity in response
/// to the keyboard keys being pressed. ECRotater allows you to rotate its controlled entity via its
/// rotateRight(), rotateLeft(), etc... functions. As you can see (like the ECFieldOFViewEmitter) some
/// Entities simply "watch" their controlled entity and emit signals when they see something interesting.
///
/// Lifetime
/// ========
/// When an Entity is deconstructed, it will deconstruct all of the EntityControllers that are
/// operating on it. This stays consistent with our general policy of "container" objects owning
/// the lifetime of their "contained" objects.
class EntityController : public QObject // inherits from QObject for 'parent-child lifetime managment' and
                                 // because most sub classes will probably need to inherit from QObject anyways
{
    Q_OBJECT
public:
    // ctor/dtor
    EntityController(Entity& entityToControl);
    virtual ~EntityController(); // dtor virtual because sub class will be used (and deleted) polymorphically

    Entity &entityControlled();
    void setEntityControlled(Entity &entityControlled);

private:
    QPointer<Entity> entityControlled_; // QPointer to prevent dangling pointer bugs (client will check this for null before using it)
};

#endif // ENTITYCONTROLLER_H
