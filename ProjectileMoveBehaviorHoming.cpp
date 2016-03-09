#include "ProjectileMoveBehaviorHoming.h"
#include "Entity.h"
#include "Projectile.h"

ProjectileMoveBehaviorHoming::ProjectileMoveBehaviorHoming(Entity *entityToFollow)
{
    this->entityToFollow_ = entityToFollow;
}

double ProjectileMoveBehaviorHoming::onMoveStep()
{
    // executed every time the HomingProectile has to move

    // get entities new position
    QPointF entityPos = entityToFollow_->pointPos();

    // face entities new pos
    QLineF line(projectile_->pointPos(),entityPos);
    line.setLength(projectile_->stepSize());
    projectile_->setFacingAngle(-1 * line.angle());

    // move forward
    double newX = projectile_->pointPos().x() + line.dx();
    double newY = projectile_->pointPos().y() + line.dy();
    projectile_->setPointPos(QPointF(newX,newY));

    return line.length();
}
