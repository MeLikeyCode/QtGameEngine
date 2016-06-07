#ifndef ITEMPUSHBACK_H
#define ITEMPUSHBACK_H

#include "EntityTargetItem.h"

class Entity;

class ItemPushback : public EntityTargetItem
{
public:
    ItemPushback();

    virtual void use(Entity *onEntity);
};

#endif // ITEMPUSHBACK_H
