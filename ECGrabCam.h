#ifndef ECGRABCAM_H
#define ECGRABCAM_H

#include <QObject>
#include "Entity.h"
#include <QPointer>
#include <QPointF>

/// An entity controller that makes the game's camera follow the entity around.
/// Each time the entity moves, the controller will make the game's camera
/// focus on that location.
///
/// Example usage:
/// ECGrabCam* c = new ECGrabCam(entity);
///
/// That is all. From then on, anytime the entity moves, the game's camera
/// will follow it.
class ECGrabCam: public QObject
{
    Q_OBJECT
public:
    ECGrabCam(Entity* entity);
public slots:
    void onEntityMoved_(Entity* entity, QPointF fromPos, QPointF toPos);
private:
    QPointer<Entity> entity_;
};

#endif // ECGRABCAM_H
