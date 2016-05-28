#include "ProjectileMoveBehaviorSine.h"
#include "Projectile.h"
#include <QPointF>
#include <QtMath> // qSin (takes radians), qDegreesToRadians
#include <QLineF>
#include "Entity.h"
#include <QDebug> // TODO: remove, test

ProjectileMoveBehaviorSine::ProjectileMoveBehaviorSine(double amplitude, double wavelength, double range, QPointF target):
    amplitude_(amplitude),
    wavelength_(wavelength),
    range_(range),
    target_(target),
    xPos_(0),
    startPosStored_(false)
{

}

/// Executed every time the projectile is asked to move.
/// Will move in a sine wave by the step size, if its moved far enough deletes projectile.
void ProjectileMoveBehaviorSine::onMoveStep()
{
    if (!startPosStored_){
        start_ = projectile_->pointPos();

        QLineF line(projectile_->pointPos(),target_);
        projectile_->setFacingAngle(line.angle() * -1);

        startPosStored_ = true;
    }

    // determine y offset at current xPos
    // y(x) = Acos(kx), A = amplitude, k = 2pi/wavelength
    double PI = 3.14159;
    double yOffset = amplitude_ * qSin((2*PI/wavelength_)*xPos_);

    QLineF startToTarget(start_, target_);

    // move straight
    QLineF moveStraightVector(startToTarget);
    moveStraightVector.setLength(projectile_->stepSize());
    double newX = projectile_->pointPos().x() + moveStraightVector.dx();
    double newY = projectile_->pointPos().y() + moveStraightVector.dy();
    projectile_->setPointPos(QPointF(newX,newY));

    // move by orthoLine
    QLineF orthoLine(moveStraightVector.p2(),target_);
    orthoLine.setLength(qAbs(yOffset));
    if (yOffset > 0){
        orthoLine.setAngle(startToTarget.angle()+90);
    } else{
        orthoLine.setAngle(startToTarget.angle()-90);
    }
    double newX2 = projectile_->pointPos().x() + orthoLine.dx();
    double newY2 = projectile_->pointPos().y() + orthoLine.dy();
    projectile_->setPointPos(QPointF(newX2,newY2));

//    // shift down by half amplitude
//    QLineF shiftLine(currLine.p2(),target_);
//    shiftLine.setLength(amplitude_);
//    shiftLine.setAngle(shiftLine.angle() - 90);
//    double newX3 = projectile_->pointPos().x() + shiftLine.x1();
//    double newY3 = projectile_->pointPos().y() + shiftLine.y1();
//    projectile_->setPointPos(QPointF(newX3,newY3));

    // increment x
    xPos_ += projectile_->stepSize();

    // if moved far enough
//    if (xPos_ > range_){
//        projectile_->deleteLater(); // give collision a chance
//    }

}
