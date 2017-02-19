#include "SpearProjectile.h"
#include "StraightMover.h"
#include "CBDamage.h"
#include "DRBDestroyProjectile.h"
#include <QPixmap>
#include "Sprite.h"
#include "Utilities.h"

SpearProjectile::SpearProjectile(double range, double damage, std::unordered_set<Entity*> noDmgList):
    Projectile(nullptr, new CBDamage(0,damage), nullptr,noDmgList),
    range_(range),
    distTravelledSoFar_(0)
{
    // set straight mover
    StraightMover* sm = new StraightMover(this);
    sm->setFaceTarget(true);

    DRBDestroyProjectile d;

    // TODO: move to base class (if this needs to happen with all projectiles)
    setRotationPoint(QPointF(0,sprite()->boundingRect().height()/2));

    // listen to movement, when moved enough, tell mover to stop moving
    connect(this, &Entity::moved,this,&SpearProjectile::onMoved_);
}

/// Executed every time the SpearProjectile moves.
/// Will detect if it has moved enough (exeded range), if so, will stop it and
/// invoke its DestReachedBehavior.
void SpearProjectile::onMoved_(Entity *entity, QPointF fromPos, QPointF toPos)
{
    Q_UNUSED(entity)

    distTravelledSoFar_ += distance(fromPos,toPos); // update distance travelled
}
