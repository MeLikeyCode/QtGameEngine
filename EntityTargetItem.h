#ifndef ENTITYTARGETITEM_H
#define ENTITYTARGETITEM_H

#include "Item.h"

class Entity;

/// An abstract that represents an Item that can be used on an Entity.
/// @author Abdullah Aghazadah
/// @date 2/13/15
class EntityTargetItem : public Item
{
public:
    virtual void use(Entity* onEntity) = 0;
};

#endif // ENTITYTARGETITEM_H
