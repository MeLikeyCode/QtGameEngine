#include "ProjectileMoveBehaviorSpinning.h"
#include <QLineF>
#include "Projectile.h"

ProjectileMoveBehaviorSpinning::ProjectileMoveBehaviorSpinning(double range, QPointF target):
    range_(range),
    target_(target),
    distanceMoved_(0),
    rotation_(0),
    moveAngleCalculated_(false)
{
    // empty constructor body
}

/// Executed every time the projectile is asked to move.
/// Will move straight by the step size and rotate it.
/// If the projectile has moved far enough, its asked to delete itself.
void ProjectileMoveBehaviorSpinning::onMoveStep()
{
    // calculate move angle
    if (!moveAngleCalculated_){
        QLineF line(projectile_->pointPos(),target_);
        moveAngle_ = line.angle();
        moveAngleCalculated_ = true;
    }

    // set next rotation
    projectile_->setFacingAngle(rotation_);
    rotation_ += 15;

    // move forward by step size
    QLineF line(projectile_->pointPos(),QPointF(-3,-3));
    line.setAngle(moveAngle_);
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
