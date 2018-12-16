#include "MCSpawner.h"

#include "Entity.h"
#include "Map.h"
#include "AngledSprite.h"
#include "ECBodyThruster.h"
#include "SpriteSheet.h"

using namespace qge;

/// Creates a MCSpawner.
/// @param map The map that the MCSpawner will spawn Entities in.
/// @param region The region in the map that entities will spawn in.
/// @param max The maximum number of Entities that the MCSpawner will create. When
/// some of the Entities die, the MCSpawner will continue to produce new ones.
/// @param numPerSec The number of Entities that the MCSpawner will create per second (until max is reached).
/// @param entityCreator The EntityCreator the MCSpawner will use to create the Entities.
MCSpawner::MCSpawner(Map *map, const QRectF &region, int max, double numPerSec, EntityCreator* entityCreator):
    map_(map),
    region_(region),
    entityCreator_(entityCreator),
    max_(max),
    ratePerSec_(numPerSec),
    timer_(new QTimer(this))
{
    connect(timer_,&QTimer::timeout,this,&MCSpawner::onTimeout_);
    turnOn();
}

/// Makes it so that the MCSpawner continues producing entities.
void MCSpawner::turnOn()
{
    double delayS = 1.0 / ratePerSec_;
    double delayMS = delayS * 1000;
    timer_->start(delayMS);
}

/// Stops the MCSpawner from making additional entities.
void MCSpawner::turnOff()
{
    timer_->stop();
}

void MCSpawner::onTimeout_()
{
    // get random point in region
    QPointF randPoint = random_.randQPointF(region_);

    Entity* entity = entityCreator_->createEntity();
    Game::game->diplomacyManager().setRelationship(entity->group(),1,Relationship::ENEMY); // TODO, WHAT?
    map_->addEntity(entity);
    entity->setPos(randPoint);
}
