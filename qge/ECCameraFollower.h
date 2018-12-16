#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

namespace qge{

/// An entity controller that makes the game's camera follow the entity around.
/// Each time the entity moves, the controller will make the game's camera
/// focus on that location.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECGrabCam* c = new ECGrabCam(entity);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// That is all. From then on, anytime the entity moves, the game's camera
/// will follow it.
class ECCameraFollower: public EntityController
{
    Q_OBJECT
public:
    ECCameraFollower(Entity* entity);
public slots:
    void onEntityMoved_(Entity* entity, QPointF fromPos, QPointF toPos);
};

}
