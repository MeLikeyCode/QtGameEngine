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
    horMoveVecCalculated_(false),
    distanceMoved_(0)
{

}

/// Executed every time the projectile is asked to move.
/// Will move in a sine wave by the step size, if its moved far enough deletes projectile.
void ProjectileMoveBehaviorSine::onMoveStep()
{
    // calculate horMoveVec and face target, once
    if (!horMoveVecCalculated_){
        QLineF line(projectile_->pointPos(),target_);
        line.setLength(projectile_->stepSize());
        horMoveVector_ = line;

        projectile_->setFacingAngle(line.angle() * -1);

        horMoveVecCalculated_ = true;
    }

    // every call

    // move by horizontal vector
    projectile_->setPointX(projectile_->pointX()+horMoveVector_.dx());
    projectile_->setPointY(projectile_->pointY()+horMoveVector_.dy());

    distanceMoved_ += projectile_->stepSize();

    // calculate vertical vector, move by it

    // determine y offset at current xPos
    // y(x) = Acos(kx), A = amplitude, k = 2pi/wavelength
    double PI = 3.14159;
    double yOffset = amplitude_ * qSin((2*PI/wavelength_)*distanceMoved_);

    QLineF verMoveVector = horMoveVector_;
    if (yOffset > 0){
        verMoveVector.setAngle(verMoveVector.angle()+90);
    }
    else{
        verMoveVector.setAngle(verMoveVector.angle()-90);
    }

    projectile_->setPointX(projectile_->pointX()+verMoveVector.dx());
    projectile_->setPointY(projectile_->pointY()+verMoveVector.dy());

    // if moved far enough
    if (distanceMoved_ > range_){
        projectile_->deleteLater(); // give collision a chance
    }

}
