#include "SpearProjectile.h"
#include "ECStraightMover.h"
#include "CBDamage.h"
#include "DRBDestroyProjectile.h"
#include <QPixmap>
#include "Sprite.h"
#include "Utilities.h"
#include "TopDownSprite.h"

SpearProjectile::SpearProjectile(double range, double damage):
    Projectile(nullptr, nullptr, new DRBDestroyProjectile()),
    range_(range),
    distTravelledSoFar_(0)
{
    // defaults
    setSprite(new TopDownSprite(QPixmap(":/resources/graphics/weapons/spear.png")));
    setSpeed(1000);

    // set Mover
    ECStraightMover* sm = new ECStraightMover(this);
    sm->setFaceTarget(true);
    setMover(sm);

    // set CollisionBehavior
    CBDamage* cb =  new CBDamage(this->health()+1,damage);
    setCollisionBehavior(cb);

    // TODO: move to base class (if this needs to happen with all projectiles)
    setRotationPoint(QPointF(0,sprite()->boundingBox().height()/2));
}

void SpearProjectile::shootTowards(const QPointF &pos)
{
    // move pos such that its right at the end of its range
    QLineF shootLine(this->pos(),pos);
    shootLine.setLength(range_);

    // delegate to base
    Projectile::shootTowards(shootLine.p2());
}
