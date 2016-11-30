#ifndef ABILITY_H
#define ABILITY_H

class Entity;
class Sprite;

/// An abstract class that represents an ability.
/// An Ability has an owner and an optional icon.
/// Before any Ability can be used, the owner has to be in a Map.
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
