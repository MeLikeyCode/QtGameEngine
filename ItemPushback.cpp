#include "ItemPushback.h"
#include "DynamicEntity.h"
#include <cassert>
#include "Inventory.h"

ItemPushback::ItemPushback()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":resources/graphics/misc/pushback.png"));
    setSprite(sprite);
}

void ItemPushback::use(Entity *onEntity)
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    DynamicEntity* owner = inventory()->entity();
    assert(owner != nullptr);

    // move entity AMOUNT pixels along owner-entity line
    int AMOUNT = 250;
    QLineF line(owner->pointPos(),onEntity->pointPos());
    line.setLength(line.length()+AMOUNT);
    QLineF line2(onEntity->pointPos(),line.p2());
    onEntity->setPointX(onEntity->pointX() + line2.dx());
    onEntity->setPointY(onEntity->pointY() + line2.dy());
}
