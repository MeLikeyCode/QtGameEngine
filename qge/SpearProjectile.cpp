#include "SpearProjectile.h"

#include <QPixmap>

#include "ECStraightMover.h"
#include "CBDamage.h"
#include "DRBDestroyProjectile.h"
#include "Sprite.h"
#include "Utilities.h"
#include "TopDownSprite.h"

using namespace qge;

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
    setMover(sm);

    // set CollisionBehavior
    CBDamage* cb =  new CBDamage(this->health()+1,damage);
    setCollisionBehavior(cb);

    setOrigin(QPointF(0,sprite()->boundingBox().height()/2));
}

void SpearProjectile::shootTowards(const QPointF &pos)
{
    // move pos such that its right at the end of its range
    QLineF shootLine(this->pos(),pos);
    shootLine.setLength(range_);

    // delegate to base
    Projectile::shootTowards(shootLine.p2());
}
