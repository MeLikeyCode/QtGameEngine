#ifndef BODYTHRUST_H
#define BODYTHRUST_H

#include "NoTargetAbility.h"
#include <QObject>
#include <QPointF>

class QTimer;
class Sound;

/// An Ability that causes the owner to thrust their body forward.
/// @author Abdullah Aghazadah
/// @date 11/18/16
class BodyThrust: public QObject, public NoTargetAbility {
    Q_OBJECT
public:
    // constructor
    BodyThrust(DynamicEntity* owner);

    virtual void use();

    double thrustDistance();
    double thrustSpeed();
    void setThrustSpeed(double speed);
    void setThrustDistance(double distance);

public slots:
    void thrustStep_();
private:
    QTimer* timer_;
    int thrustStepFrequency_;
    int maxThrustSteps_;
    int currentThrustStep_;
    double thrustLengthEachStep_;
    double thrustDistance_; // calculate from other attributes, here for perf
    double thrustSpeed_; // calculated from other attributes, here for perf
    bool headingForward_; // is the spear heading forward or backward?
    bool headingBackward_;
    bool headingBackwardDueToCollision_;
    bool alreadyThrusting_;
    void resetVariables();
    QPointF collisionPoint_;

    double damage_;

    Sound* soundEffect_;
};

#endif // BODYTHRUST_H
