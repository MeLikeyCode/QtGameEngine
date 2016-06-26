#include "ItemTeleport.h"
#include "Sprite.h"
#include "DynamicEntity.h"
#include <cassert>
#include "Inventory.h"
#include "Map.h"

ItemTeleport::ItemTeleport()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":resources/graphics/misc/teleport.png"));
    setSprite(sprite);
}

void ItemTeleport::use(const QPointF &point)
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    DynamicEntity* owner = inventory()->entity();
    assert(owner != nullptr);

    // move owner to the specified location
    owner->setPointPos(point);

    // play a "poof" animation at the location of the owner
    Sprite* poofSprite = new Sprite();
    poofSprite->addFrames(":resources/graphics/effects/poof",10,"poof");
    owner->map()->playOnce(poofSprite,"poof",50,owner->pointPos());
}
