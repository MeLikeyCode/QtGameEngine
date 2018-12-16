#include "ECMouseFacer.h"

#include "ECRotater.h"
#include "Map.h"

using namespace qge;

ECMouseFacer::ECMouseFacer(Entity *entity):
    EntityController(entity),
    rotateStepSize_(5),
    rotateFrequency_(30),
    rotateTimer_(new QTimer(this)),
    rotater_(new ECRotater(entity))
{
    rotater_->setParent(this);

    connect(rotateTimer_,&QTimer::timeout,this,&ECMouseFacer::rotateStep_);
    rotateTimer_->start(rotateFrequency_);
}

/// Executed whenever the entity_ needs to rotate.
/// Will rotate it closer to the mouse.
void ECMouseFacer::rotateStep_()
{
    // if the entity has been destroyed, stop rotating
    Entity* entity = entityControlled();
    if (entity == nullptr){
        rotateTimer_->disconnect();
        return;
    }

    // do nothing if entity is not in a map
    Map* entitysMap = entity->map();
    if (entitysMap == nullptr)
        return;

    // do nothing if the entity's map isn't game's current map
    if (entitysMap->game() == nullptr)
        return;

    // TODO: ECRotater is way too slow right now.
    // When you speed it up, enable the line immediately below and disable all lines after that (in this function)
    // rotater_->rotateTowards(entitysMap->getMousePosition());
    QLineF entityToMousePos(entity->pos(),entitysMap->getMousePosition());
    entity->setFacingAngle(-1 * entityToMousePos.angle());
}
