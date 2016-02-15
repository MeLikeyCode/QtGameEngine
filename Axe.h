#ifndef AXE_H
#define AXE_H

// inherits
#include "MeleeWeapon.h"

// value members
#include <QTimer>

/// Represents an Axe.
/// @author Abdullah Aghazadah
/// @date 1-9-16
class Axe: public QObject, public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    Axe();

    void attack();

public slots:
    void swingStep();
private:
    QTimer* timer_;

    double swingAngleEachStep_;
    int swingFrequency_; // how often we step
    int numOfSwingStepsBackward_; // how many steps to swing backwards
    int numOfSwingStepsForward_; // how many steps to swing forward

    bool swingingOutPhase_;
    bool swingingForwardPhase_; // are we swinging forward or backward in this step?
    bool headingBackwardPhase_; // are we heading bakcward after a full swing?
    bool alreadySwinging_; // has the weapon already begun swinging?

    int numOfCurrentSwingSteps_; // how many swing steps have we taken
};

#endif // AXE_H
