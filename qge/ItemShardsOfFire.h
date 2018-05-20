#pragma once

#include "NoTargetItem.h"

namespace qge{

class ShardsOfFireAbility;

class ItemShardsOfFire : public NoTargetItem
{
public:
    ItemShardsOfFire(int numShards = 10, double shardDistance = 600);
    void use_() override;
};

}
