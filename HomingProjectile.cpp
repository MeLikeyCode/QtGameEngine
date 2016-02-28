#include "HomingProjectile.h"
#include "Map.h"

HomingProjectile::HomingProjectile(Entity *entityTarget)
{
    // default sprite (spear.png)
    Sprite* spr_ = new Sprite();
    QPixmap pix(":resources/graphics/weapons/axe.png");
    spr_->setPixmap(pix);
    setSprite(spr_);

    // defaults
    setStepSize(15);
    setStepFrequency(50);
    entityTarget_ = entityTarget;
    setTargetPoint(entityTarget->pointPos());

    faceEntity_();
}

void HomingProjectile::moveStep()
{
    faceEntity_();
    setTargetPoint(entityTarget_->pointPos());

    // move at current angle
    QLineF line(this->pointPos(),QPointF(-500,-500));
    line.setAngle(-1 * this->facingAngle());
    line.setLength(this->stepSize());

    double newX = this->pointPos().x() + line.dx();
    double newY = this->pointPos().y() + line.dy();

    this->setPointPos(QPointF(newX,newY));
}

void HomingProjectile::collidedWith(std::vector<Entity *> entities)
{
    // TODO: implement
    return;
}

void HomingProjectile::targetReached()
{
    // delete projectile
    map()->removeEntity(this);
    delete this;
}

void HomingProjectile::faceEntity_()
{
    QLineF line(pointPos(),entityTarget_->pointPos());
    setFacingAngle(-1 * line.angle());
}
