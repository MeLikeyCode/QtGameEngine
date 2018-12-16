#pragma once

#include "Vendor.h"

#include "Ability.h"

namespace qge{

/// An Ability that can simply be used with no target.
/// @author Abdullah Aghazadah
class NoTargetAbility : public Ability {
public:
    NoTargetAbility(Entity* owner, QPixmap icon = QPixmap());

    void use();

protected:
    // uses template method pattern (use does some common work, then delgates
    // to virtual function)
    virtual void useImplementation() = 0;
};

}
