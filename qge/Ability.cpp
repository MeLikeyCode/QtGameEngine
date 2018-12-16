#include "Ability.h"

using namespace qge;

/// Returns the owner of the Ability.
Entity *Ability::owner()
{
    return owner_;
}

/// Sets the owner of the Ability.
void Ability::setOwner(Entity *owner)
{
    owner_ = owner;
}

/// Returns the icon of the Ability (or null pixmap if it has no icon).
QPixmap Ability::icon()
{
    return icon_;
}

/// Sets the icon of the Ability.
void Ability::setIcon(const QPixmap &icon)
{
    icon_ = icon;
}

std::string Ability::description()
{
    return description_;
}

void Ability::setDescription(const std::string &description)
{
    description_ = description;
}

/// Constructs an Ability with the specified owner and icon.
Ability::Ability(Entity *owner, QPixmap icon):
    owner_(owner),
    icon_(icon),
    description_("No description.")
{
    // empty ctor body
}

/// Asserts that the owner of the Ability is still alive.
/// This function should be called by sub classes before actually
/// executing the use code.
void Ability::assertOwnerIsAlive_()
{
    assert(owner_ != nullptr);
}
