#include "ItemPushback.h"

#include "Entity.h"
#include "Inventory.h"
#include "Sprite.h"
#include "TopDownSprite.h"

using namespace qge;

ItemPushback::ItemPushback()
{
    // set sprite
    TopDownSprite* sprite = new TopDownSprite(QPixmap(":resources/graphics/misc/pushback.png"));
    setSprite(sprite);
}

void ItemPushback::use_(Entity *onEntity)
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    // move entity AMOUNT pixels along owner-entity line
    int AMOUNT = 250;
    QLineF line(owner->pos(),onEntity->pos());
    line.setLength(line.length()+AMOUNT);
    QLineF line2(onEntity->pos(),line.p2());
    onEntity->setX(onEntity->x() + line2.dx());
    onEntity->setY(onEntity->y() + line2.dy());
}
