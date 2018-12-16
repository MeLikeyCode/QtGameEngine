#pragma once

#include "Vendor.h"

#include "NoTargetAbility.h"

namespace qge{

class Sound;

class ShardsOfFireAbility : public NoTargetAbility
{
public:
    ShardsOfFireAbility(int numShards, double shardDistance, Entity* owner=nullptr);
    void useImplementation() override;

private:
    Sound* soundEffect_;
    int numShards_;
    double shardDistance_;
};

}
