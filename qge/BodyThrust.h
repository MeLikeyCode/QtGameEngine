#pragma once

#include "Vendor.h"

#include "NoTargetAbility.h"
#include "PlayingAnimationInfo.h"

class QTimer;

namespace qge{

class Sound;

/// An Ability that causes the using entity to thrust its body forward at its current
/// facing angle and then back. You can optionally specify an animation of the Entity
/// that will be played when its thrusting.
/// @author Abdullah Aghazadah
/// @date 11/18/16
class BodyThrust: public NoTargetAbility {
    Q_OBJECT
public:
    // constructor
    BodyThrust(Entity* owner);

    virtual void useImplementation();

    double thrustDistance();
    double thrustSpeed();
    void setThrustSpeed(double speed);
    void setThrustDistance(double distance);
    bool thrusting() const;

    void setAnimationToPlayWhileThrusting(const std::string& animation);

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
    bool damaged_;
    bool headingForward_; // is the spear heading forward or backward?
    bool headingBackward_;
    bool headingBackwardDueToCollision_;
    bool alreadyThrusting_;
    void resetVariables();
    QPointF collisionPoint_;

    double damage_;
    std::string animationToPlay_;
    PlayingAnimationInfo lastAnim_;

    Sound* soundEffect_;

    void done_();
};

}
