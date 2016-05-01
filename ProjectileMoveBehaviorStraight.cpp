#include "ProjectileMoveBehaviorStraight.h"
#include "Projectile.h"

ProjectileMoveBehaviorStraight::ProjectileMoveBehaviorStraight(double range, QPointF target):
    range_(range),
    target_(target),
    distanceMoved_(0)
{

}

/// Executed every time the projectile is asked to move.
/// Will move straight by the step size, if its moved far enough deletes projectile.
void ProjectileMoveBehaviorStraight::onMoveStep()
{
    // move forward by step size
    QLineF line(projectile_->pointPos(),target_);
    projectile_->setFacingAngle(-1 * line.angle()); // make sure projectile faces target_
    line.setLength(projectile_->stepSize());
    double newX = projectile_->pointPos().x() + line.dx();
    double newY = projectile_->pointPos().y() + line.dy();
    projectile_->setPointPos(QPointF(newX,newY));
    distanceMoved_ += line.length();

    // if moved far enough (distanceMoved exceeds range)
    if (distanceMoved_ > range_){
        projectile_->deleteLater(); // give collision a chance
    }
}
