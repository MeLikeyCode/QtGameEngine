#ifndef AXE_H
#define AXE_H

#include "MeleeWeapon.h"

class QTimer;

/// Represents an Axe.
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
    double damage_;

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
};

#endif // AXE_H
