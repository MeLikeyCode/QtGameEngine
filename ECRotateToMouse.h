#ifndef ECROTATETOMOUSE_H
#define ECROTATETOMOUSE_H

#include <QObject>
#include <QPointer>
#include "Entity.h"
#include <memory>

class QTimer;
class ECRotater;

/// An entity controller (TODO: link to doc) that makes an Entity constantly rotate
/// towards the mouse.
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
class ECRotateToMouse: public QObject // inherits from QObject so that we can use the 'parent manages lifetime of child' functionality for fields that are QObjects
{
    Q_OBJECT
public:
    ECRotateToMouse(Entity &entity);

public slots:
    void rotateStep_();
private:
    QPointer<Entity> entity_;
    double rotateStepSize_; // in degrees
    double rotateFrequency_; // in ms

    QTimer* rotateTimer_;

    std::unique_ptr<ECRotater> rotater_;
};

#endif // ECROTATETOMOUSE_H
