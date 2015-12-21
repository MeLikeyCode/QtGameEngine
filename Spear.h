#ifndef SPEAR_H
#define SPEAR_H

// inherits
#include "MeleeWeapon.h"

// value members
#include <QTimer>

class Spear: public QObject, public MeleeWeapon{
    Q_OBJECT
public:
    // constructor
    Spear();

    void attackThrust();
    QPointF attachmentPoint();

    void setAttachmentPoint(QPointF point);

    void setTip(QPointF point);
    void resetTip();
    QPointF tip();

    void setLength(double length);
    double length();
    void setWidth(double width);
    double width();

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
    QPixmap pm_; // allows scaling
    Sprite* spr_;
    QPointF tip_;
    QPointF attachmentPoint_;
};

#endif // SPEAR_H
