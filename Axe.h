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
    QPointF attachmentPoint();

    void setAttachmentPoint(QPointF point);

    void setTip(QPointF point);
    void resetTip();
    QPointF tip();

    void setLength(double length);
    double length();
    void setWidth(double width);
    double width();

    double totalSwingAgnel();
    double swingSpeed();
    void setSwingingSpeed(double speed);
    void setTotalSwingAngle(double distance);

public slots:
    void swingStep();
private:
    QTimer* timer_;
    int swingStepFrequency_;
    int maxSwingSteps_;
    int currentSwingStep_;
    double swingAngleEachStep_;
    double totalSwingAngle_; // calculate from other attributes, here for perf
    double swingSpeed_; // calculated from other attributes, here for perf
    bool swingingForward_; // is axe swinging forward or heading backward?
    bool headingBackward_;
    bool headingBackwardDueToCollision_;
    bool alreadySwinging_;
    bool swingingOut_; // is the axe in its initial swing out stage?
    int maxSwingOutSteps_; // how many steps to swing out for?
    int currentSwingOutStep_;
    void resetVariables();
    QPixmap pm_; // allows scaling
    Sprite* spr_;
    QPointF tip_;
    QPointF attachmentPoint_;
};

#endif // AXE_H
