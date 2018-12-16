#pragma once

#include "Vendor.h"

#include "MeleeWeapon.h"

namespace qge{

class Sound;

/// Represents a spear.
/// @author Abdullah Aghazadah
/// @date 1-9-16
class Spear: public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    Spear();

    virtual void attack(QPointF position);

    double thrustDistance();
    double thrustSpeed();
    void setThrustSpeed(double speed);
    void setThrustDistance(double distance);

public slots:
    void thrustStep();
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

    Sound* soundEffect_;
};

}
