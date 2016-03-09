#include "ProjectileMoveBehaviorStraight.h"
#include "Projectile.h"


double ProjectileMoveBehaviorStraight::onMoveStep()
{
    // executed every time the projectile needs to move forward

    QLineF line(projectile_->pointPos(),projectile_->targetPoint());
    line.setLength(projectile_->stepSize());
    projectile_->setFacingAngle(-1 * line.angle());

    double newX = projectile_->pointPos().x() + line.dx();
    double newY = projectile_->pointPos().y() + line.dy();

    projectile_->setPointPos(QPointF(newX,newY));

    return line.length();
}
