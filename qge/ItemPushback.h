#pragma once

#include "Vendor.h"

#include "EntityTargetItem.h"

namespace qge{

class Entity;

class ItemPushback : public EntityTargetItem
{
public:
    ItemPushback();

    virtual void use_(Entity *onEntity);
};

}
