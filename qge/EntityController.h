#pragma once

#include "Vendor.h"

#include "Entity.h"

namespace qge{

class Entity;

/// An object that provides some sort of behavior for an Entity (i.e "controls" the Entity in some way).
/// @author Abdullah Aghazadah
/// @date 10/6/17
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Entity* e; // assume e points to some entity
/// EntityController* c = new SomeConcreteEntityController(e);
/// c.setEntityControlled(e); // make the EntityController control Entity e
/// c.entityControlled(); // get the Entity that the EntityController is controlling/operating on
/// // Note: Concrete EntityControllers will most likely have additional methods and/or signals.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// The best way to get an idea of what kind of things EntityControllers do with their controlled entity
/// is to look at a few example EntityControllers.
///
/// @note As a naming convention, all concrete EntityControllers have the prefix EC.
///
/// ECFieldOfViewEmitter will emit a signal whenever another entity either enters or leaves its
/// controlled entity's field of view. ECMoveByKeyboardFourDirectional will move its controlled
/// entity in response to the keyboard keys being pressed. ECRotater allows you to rotate its
/// controlled entity via its rotateRight(), rotateLeft(), etc... functions.
///
/// As you can see some EntitiyControllers simply "watch" their controlled entity and emit signals
/// when they see something interesting (e.g. ECFieldOFViewEmitter), other EntityControllers allow
/// you to do something to the controlled entity (e.g. ECRotater), and some EntityControllers make
/// the controlled entity do something in response to some external event (e.g.
/// ECKeyboardMover4Directional). In summary, EntityControllers allow you to give a certain Entity
/// some functionality.
///
/// Lifetime
/// ========
/// When an Entity is deconstructed, it will deconstruct all of the EntityControllers that are
/// operating on it. This stays consistent with our general policy of "container" objects owning
/// the lifetime of their "contained" objects. We leverage the QObject parent/child system in order
/// to enforce this policy.
class EntityController : public QObject // inherits from QObject for 'parent-child lifetime managment' and
                                        // because most sub classes will probably need to inherit from QObject anyways
{
    Q_OBJECT
public:
    // ctor/dtor
    EntityController(Entity* entityToControl);
    virtual ~EntityController(); // dtor virtual because sub class will be used (and deleted) polymorphically

    Entity* entityControlled();
    void setEntityControlled(Entity* entityControlled);

    virtual void onControlledEntityDying(Entity* controlledEntity);

public slots:
    void onControlledEntityDying_(Entity* sender);

private:
    QPointer<Entity> entityControlled_; // QPointer to prevent dangling pointer issues
};

}
