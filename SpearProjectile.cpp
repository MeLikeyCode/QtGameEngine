#include "SpearProjectile.h"
#include "StraightMover.h"
#include "CBDamage.h"
#include "DRBDestroyProjectile.h"
#include <QPixmap>
#include "Sprite.h"
#include "Utilities.h"

SpearProjectile::SpearProjectile(double range, double damage, std::unordered_set<Entity*> noDmgList):
    Projectile(nullptr, new CBDamage(0,damage), new DRBDestroyProjectile(),noDmgList),
    range_(range),
    distTravelledSoFar_(0)
{
    // set sprite
    setSprite(new Sprite(QPixmap(":/resources/graphics/weapons/spear.png")));

    // set straight mover
    StraightMover* sm = new StraightMover(this);
    sm->setFaceTarget(true);
    setMover(sm);

    // TODO: move to base class (if this needs to happen with all projectiles)
    setRotationPoint(QPointF(0,sprite()->boundingRect().height()/2));
}

void SpearProjectile::shootTowards(const QPointF &pos)
{
    // move pos such that its right at the end of its range
    QLineF shootLine(pointPos(),pos);
    shootLine.setLength(range_);

    // delegate to base
    Projectile::shootTowards(shootLine.p2());
}
