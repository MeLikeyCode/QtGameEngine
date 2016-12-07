#ifndef ABILITY_H
#define ABILITY_H

class Entity;
class Sprite;

/// An abstract class that represents an ability.
/// An Ability has an owner and an optional icon.
/// The owner may be effected in some way when the ability is used, depending
/// on the ability. The Icon is used by GUI purposes (i.e.by AbilityCell).
///
/// Most abilities require the owner to be in a Map before being used.
/// The main difference between Abilities and Items is the fact that Items are
/// Entities, therefore they can be on the ground where as Abilities cannot.
/// @author Abdullah Aghazadah
class Ability
{
public:
    Entity* owner();
    void setOwner(Entity* owner);

    Sprite* icon();
    void setIcon(Sprite* sprite);

protected:
    // ctor protected so that abstract class cannot be constructed
    // yet derived classes can still call ctor
    Ability(Entity* owner, Sprite *icon = nullptr);
private:
    Entity* owner_;
    Sprite* icon_;          // nullptr = no icon
};

#endif // ABILITY_H
