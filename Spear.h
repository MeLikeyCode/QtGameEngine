#ifndef SPEAR_H
#define SPEAR_H

// inherits
#include "MeleeWeapon.h"

// value members
#include <QTimer>

class Spear: public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    Spear();

    void attackThrust();
    QPointF tip();
public slots:
    void thrustStep();
private:
    QTimer* timer_;
    int maxThrustSteps_;
    int currentThrustStep_;
    int thrustLengthEachStep_;
    bool headingForward_; // is the spear heading forward or backward?
    bool headingBackward_;
    bool alreadyThrusting_;

};

#endif // SPEAR_H
