#include "ItemShardsOfFire.h"

#include "ShardsOfFireAbility.h"

qge::ItemShardsOfFire::ItemShardsOfFire(int numShards, double shardDistance)
{
    setDescription("Spits shards of fire around the owner.");
    setAbility(new ShardsOfFireAbility(numShards,shardDistance));
}

void qge::ItemShardsOfFire::use_()
{
    // do nothing (item has ability, thus ability will be used on use())
}
