#pragma once

#include "Ability.h"

class QPointF;

namespace qge{

class PointTargetAbility: public Ability {
public:
    PointTargetAbility(Entity& owner, Sprite* icon = nullptr);

    void use(const QPointF& atPoint);

protected:
    virtual void useImplementation(const QPointF& atPoint) = 0;
};

}
