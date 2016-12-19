#ifndef ECCHECKFOV_H
#define ECCHECKFOV_H

#include <unordered_set>
#include <QObject>
#include <QPointer>
#include "Entity.h"

class QTimer;

/// An entity controller (TODO: add doc link) that checks the field of view of
/// an entity and emits a signal whenever other entities enter or leave the
/// controlled entity's field of view.
///
/// Example usage:
/// ECCheckFOV* c = new ECCheckFOV(entity);
/// connect(c,&ECCheckFOV::entityEntersFOV,this,myCallback);
/// connect(c,&ECCheckFOV::entityLeavesFOV,this,myCallback);
/// @author Abdullah Aghazadah
/// @date 11/23/16
class ECFieldOfViewEmitter: public QObject
{
    Q_OBJECT
public:
    ECFieldOfViewEmitter(Entity& entity);

public slots:
    void checkFov_();
signals:
    /// Emitted whenever an entity enters the field of view of the entity
    /// controlled.
    void entityEntersFOV(Entity* entity);

    /// Emitted whenever an entity leaves the field of view of the entity
    /// controlled.
    void entityLeavesFOV(Entity* entity);
private:
    QPointer<Entity> entity_;
    double fieldOfViewAngle_;
    double fieldOfViewDistance_;
    double fieldOfViewCheckFrequency_;
    QTimer* timerCheckFov_;

    // helper
    std::unordered_set<Entity*> entitiesInView_();
    std::unordered_set<Entity*> entitiesInViewLastTime_;
};

#endif // ECCHECKFOV_H
