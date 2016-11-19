#ifndef BODYTHRUST_H
#define BODYTHRUST_H

// inherits
#include "MeleeWeapon.h"

// value members
#include <QTimer>

class Sound;

/// Represents a weapon that causes the owner to thrust their body forward.
/// This Item does not have any art (i.e. it is invisible) because it simply
/// uses the entity's own body.
/// @author Abdullah Aghazadah
/// @date 11/18/16
class BodyThrust: public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    BodyThrust();

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

    double damage_;

    Sound* soundEffect_;
};

#endif // BODYTHRUST_H
