#ifndef ENTITYTARGETITEM_H
#define ENTITYTARGETITEM_H

#include "Item.h"

class Entity;

/// Represents an Item that can be used on an Entity.
/// @author Abdullah Aghazadah
/// @date 2/13/15
class EntityTargetItem : public Item
{
public:
    EntityTargetItem();
    void use(Entity* onEntity);
};

#endif // ENTITYTARGETITEM_H
