#include "StraightProjectile.h"
#include <QLineF>
#include "Map.h"
#include <QDebug> // TODO: remove


StraightProjectile::StraightProjectile(QPointF startPoint, QPointF targetPoint)
{
    // default sprite (spear.png)
    Sprite* spr_ = new Sprite();
    spr_->setPixmap(QPixmap(":resources/graphics/weapons/spear.png"));
    setSprite(spr_);

    // defaults
    setStepSize(50);
    setStepFrequency(70);
    setPointPos(startPoint);
    setTargetPoint(targetPoint);

    QLineF line(startPoint,targetPoint);
    setFacingAngle(-1 * line.angle());
}



void StraightProjectile::moveStep()
{
    // executed every time the projectile needs to move forward
    // -create line from pos to target
    // -set dir of line same as facing angle of projectile
    // -set lenght of line same as stepsize of projectile
    // - move by dx, dy of line

//    // make sure projectile faces facing point
//    QLineF line(this->pointPos(),this->targetPoint());
//    this->setFacingAngle(-1* line.angle());
//    line.setLength(this->stepSize());

    QLineF line(this->pointPos(),QPointF(-500,-500));
    line.setAngle(-1 * this->facingAngle());
    line.setLength(this->stepSize());

    double newX = this->pointPos().x() + line.dx();
    double newY = this->pointPos().y() + line.dy();

    this->setPointPos(QPointF(newX,newY));
}

void StraightProjectile::collidedWith(std::vector<Entity *> entities)
{
    // executed every time the projectile collides with some entities
    // - traverse through, if not on nodamage list, kill it
    for (Entity* entity:entities){
        if (!isInNoDamageList(entity)){
            map()->removeEntity(entity);
            delete entity;
        }
    }
}

void StraightProjectile::targetReached()
{
    // delete projectile
    map()->removeEntity(this);
    delete this;
}
