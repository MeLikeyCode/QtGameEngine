#include "ItemShardsOfFire.h"

#include <cassert>

#include "TopDownSprite.h"
#include "ShardsOfFireAbility.h"
#include "Inventory.h"

qge::ItemShardsOfFire::ItemShardsOfFire(int numShards, double shardDistance):
    numShards_(numShards),
    shardDistance_(shardDistance)
{
    setSprite(new TopDownSprite(QPixmap(":/resources/graphics/effects/fireRain.png")));
    setDescription("Spits shards of fire around the owner.");
    ability_ = new ShardsOfFireAbility(numShards,shardDistance);
}

void qge::ItemShardsOfFire::use_()
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    ability_->setOwner(inventory()->entity());
    ability_->use();
}
