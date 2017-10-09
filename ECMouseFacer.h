#ifndef ECROTATETOMOUSE_H
#define ECROTATETOMOUSE_H

#include <QPointer>
#include <memory>

#include "EntityController.h"
#include "Entity.h"

class QTimer;
class ECRotater;

/// An entity controller (TODO: link to doc) that makes an Entity constantly rotate
/// towards the mouse.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECRotateToMouse* c = new ECRotateToMouse(entity);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// That is all that is needed. Once constructed, the entity passed into the
/// constructor will contineously rotate to face the mouse.
/// @author Abdullah Aghazadah
/// @date 11/22/16
class ECMouseFacer: public EntityController
{
    Q_OBJECT
public:
    ECMouseFacer(Entity* entity);

public slots:
    void rotateStep_();
private:
    double rotateStepSize_; // in degrees
    double rotateFrequency_; // in ms

    QTimer* rotateTimer_;

    std::unique_ptr<ECRotater> rotater_;
};

#endif // ECROTATETOMOUSE_H
