#ifndef POINTTARGETABILITY_H
#define POINTTARGETABILITY_H

#include "Ability.h"

class QPointF;

class PointTargetAbility: public Ability {
public:
    PointTargetAbility(Entity& owner, Sprite* icon = nullptr);

    void use(const QPointF& atPoint);

protected:
    virtual void useImplementation(const QPointF& atPoint) = 0;
};

#endif // POINTTARGETABILITY_H
