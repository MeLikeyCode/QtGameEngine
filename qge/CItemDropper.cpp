#include "CItemDropper.h"

#include "STLWrappers.h"
#include "Entity.h"
#include "Map.h"
#include "ItemGold.h"
#include "ItemHealthPotion.h"
#include "Utilities.h"

using namespace qge;
using namespace STLWrappers;

void CItemDropper::addEntity(Entity *entity)
{
    // do nothing if entity already added (to prevent double-listening)
    if (contains(entities_,entity))
        return;

    // listen to when entity dies (so we can drop an item)
    connect(entity,&Entity::dying,this,&CItemDropper::onEntityDies_);
}

void CItemDropper::removeEntity(Entity *entity)
{
    // note: don't have to handle case of entity already being removed
    // - set handles this for us and double-unlistening is ok (does nothing)

    // stop listening
    disconnect(entity,&Entity::dying,this,&CItemDropper::onEntityDies_);

    // remove
    remove(entities_,entity);
}

/// Executed when an Entity added to the CItemDropper dies.
/// We will drop a random item at its location.
void CItemDropper::onEntityDies_(Entity *entity)
{
    // do nothing if the entity is not in a map
    Map* entitysMap = entity->map();
    if (entitysMap == nullptr)
        return;

    // other wise, drop an item
    int r = randInt(0,1);
    if (r == 0){
        ItemGold* gold = new ItemGold();
        gold->setPos(entity->pos());
        entitysMap->addEntity(gold);
    }
    else if (r == 1){
        ItemHealthPotion* hp = new ItemHealthPotion(20);
        hp->setPos(entity->pos());
        entitysMap->addEntity(hp);
    }


    // stop listening for when this entity dies (this wil happen automatically when the entity (QObject) gets destroyed
}
