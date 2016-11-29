#include "ECRotateToMouse.h"
#include <cassert>
#include <QTimer>
#include "ECRotater.h"
#include "Map.h"

ECRotateToMouse::ECRotateToMouse(Entity *entity):
    entity_(entity),
    rotateStepSize_(5),
    rotateFrequency_(45),
    rotateTimer_(new QTimer(this)),
    rotater_(new ECRotater(entity))
{
    // make sure passed in entity is not nullptr
    assert(entity != nullptr);

    rotater_->setParent(this);

    connect(rotateTimer_,&QTimer::timeout,this,&ECRotateToMouse::rotateStep_);
    rotateTimer_->start(rotateFrequency_);
}

ECRotateToMouse::~ECRotateToMouse()
{
    // make sure rotateTimer_ gets delete
    // - it will (child of this qobject)

    // make sure rotater_ gets deleted
    // - it will since its a child qobject
}

/// Executed whenever the entity_ needs to rotate.
/// Will rotate it closer to the mouse.
void ECRotateToMouse::rotateStep_()
{
    // if the entity has been destroyed, stop rotating
    if (entity_.isNull()){
        rotateTimer_->disconnect();
        return;
    }

    // do nothing if entity is not in a map
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    rotater_->rotateTowards(entitysMap->getMousePosition());
}
