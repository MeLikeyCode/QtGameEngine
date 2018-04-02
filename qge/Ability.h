#pragma once

#include <QPointer>

#include "Entity.h"
#include "Sprite.h"

namespace qge{

/// An abstract class that represents an ability.
/// An Ability has an owner and an icon.
/// The owner may be effected in some way when the ability is used, depending
/// on the ability. The Icon is used for GUI purposes (i.e.by AbilityCell).
///
/// All Abilities will throw assertion errors if they are used after the owner is dead
/// (this is automatically handled by NoTargetAbility, PointTargetAbility, and EntityTargetAbility).
/// Most abilities require the owner to be in a Map before being used, but not all, so
/// this is something that each Ability subclass should handle on its own.
/// The main difference between Abilities and Items is the fact that Items are
/// Entities, therefore they can be on the ground where as Abilities cannot.
/// @author Abdullah Aghazadah
class Ability : public QObject // need QPointers to Ability
{
    Q_OBJECT
public:
    // owner
    Entity* owner();
    void setOwner(Entity& owner);

    // icon
    Sprite* icon();
    void setIcon(Sprite* sprite);

protected:
    // ctor protected so that abstract class cannot be constructed
    // yet derived classes can still call ctor
    Ability(Entity& owner, Sprite *icon = nullptr);

    void assertOwnerIsAlive_();
private:
    QPointer<Entity> owner_;
    QPointer<Sprite> icon_;
};

}
