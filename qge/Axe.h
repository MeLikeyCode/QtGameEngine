#pragma once

#include "Vendor.h"

#include "MeleeWeapon.h"

class QTimer;

namespace qge{

class Sound;

/// Represents an Axe.
/// An Axe is a MeleeWeapon that swings and executes its CollisionBehavior
/// when it hits stuff.
/// @author Abdullah Aghazadah
/// @date 1-9-16
class Axe: public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    Axe();

    virtual void attack(QPointF position);

public slots:
    void swingStep();
private:
    QTimer* timer_;

    bool alreadySwinging_; // has the weapon already begun swinging?
    double swingAngleEachStep_;
    int swingFrequency_; // how often we step
    int maxDrawBackSteps_; // how many steps to initially swing out
    int currentDrawBackStep_;
    int maxDrawForwardSteps_;
    int currentDrawForwardStep_;
    int maxForwardSteps_; // how many steps to go after coming back from draw
    int currentForwardStep_;
    int maxBackwardSteps_;
    int currentBackwardStep_;
    int stepsToGoBackwardToNeutral_; // if we hit someting going forward,
                                     // how many steps till we get back to neutral?
    int currentStepToGoingBackToNeutral_;

    bool hitSomethingComingBackFromDraw_;
    bool hitSomethingDuringForwardStep_;

    Sound* soundEffect_;
};

}
