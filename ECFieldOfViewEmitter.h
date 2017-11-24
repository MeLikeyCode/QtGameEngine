#ifndef ECCHECKFOV_H
#define ECCHECKFOV_H

#include <unordered_set>
#include <QPointer>
#include <QGraphicsPolygonItem>

#include "EntityController.h"
#include "Entity.h"

class QTimer;

/// An entity controller that checks the field of view of
/// an entity and emits a signal whenever other entities enter or leave the
/// controlled entity's field of view. You can also use entitiesInView() to get
/// all the current entities in the controlled entity's field of view.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECCheckFOV* c = new ECCheckFOV(entity);
/// connect(c,&ECCheckFOV::entityEntersFOV,this,myCallback);
/// connect(c,&ECCheckFOV::entityLeavesFOV,this,myCallback);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @author Abdullah Aghazadah
/// @date 11/23/16
class ECFieldOfViewEmitter: public EntityController
{
    Q_OBJECT
public:
    ECFieldOfViewEmitter(Entity* entity);
    std::unordered_set<Entity*> entitiesInView();

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
    double fieldOfViewAngle_;
    double fieldOfViewDistance_;
    double fieldOfViewCheckFrequency_;
    QTimer* timerCheckFov_;

    QGraphicsPolygonItem* polyItem_; // TODO: remove, for debugging only

    // helper
    std::unordered_set<Entity*> entitiesInViewLastTime_;
};

#endif // ECCHECKFOV_H
