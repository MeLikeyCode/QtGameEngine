#include "Spear.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug>

Spear::Spear()
{
    // default sprite
    Sprite* spr = new Sprite();
    spr->setPixmap(QPixmap(":resources/graphics/weapons/spear.png"));
    setSprite(spr);

    timer_ = new QTimer(this);
    maxThrustSteps_ = 10;
    currentThrustStep_ = 0;
    thrustLengthEachStep_ = 5;
    headingBackward_ = false;
    headingForward_ = false;
    alreadyThrusting_ = false;

    addNamedPoint(QPointF(85,6),"tip");
}

void Spear::attackThrust()
{
    // if its already thrusting, don't do anything
    if (alreadyThrusting_){
        return;
    }

    headingBackward_ = false;
    headingForward_ = true;
    currentThrustStep_ = 0;
    connect(timer_,SIGNAL(timeout()),this,SLOT(thrustStep()));
    timer_->start(25);
    alreadyThrusting_ = true;
}

/// Returns the position of the tip of the spear in map coordinates.
QPointF Spear::tip()
{
    return mapToMap(namedPoint("tip"));
}

void Spear::thrustStep()
{
    // kill things that collide with the tip
    std::vector<Entity*> collidingEntities = map()->entities(tip());
    for (Entity* e: collidingEntities){
        if (e != this && e!= owner()){
            map()->removeEntity(e);
            delete e;
        }
    }

    // if moving forward
    if (headingForward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear forward at current angle
        QLineF line(pointPos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setLength(thrustLengthEachStep_);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moving backward TODO
    if (headingBackward_ && currentThrustStep_ < maxThrustSteps_){
        // move spear backward at current angle
        QLineF line(pointPos(),QPointF(1,1));
        line.setAngle(360-facingAngle());
        line.setAngle(line.angle() + 180);
        line.setLength(thrustLengthEachStep_);
        double newX = pointPos().x() + line.dx();
        double newY = pointPos().y() + line.dy();
        QPointF newPt(newX,newY);
        setPointPos(newPt);

        // update thrust step counter
        currentThrustStep_++;

        return;
    }

    // if moved forward enough, move backward
    if (headingForward_ && currentThrustStep_ >= maxThrustSteps_){
        headingForward_ = false;
        headingBackward_ = true;
        currentThrustStep_ = 0;

        return;
    }

    // if moved backward enough, stop moving
    if (headingBackward_ && currentThrustStep_ >= maxThrustSteps_){
        timer_->disconnect();

        alreadyThrusting_ = false;
        return;
    }

}
