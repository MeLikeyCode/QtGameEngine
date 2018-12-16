#include "ItemHealthPotion.h"

#include "Inventory.h"
#include "Entity.h"
#include "TopDownSprite.h"

using namespace qge;

ItemHealthPotion::ItemHealthPotion(int amountToHealBy_):
    amountToHealBy_(amountToHealBy_)
{
    TopDownSprite* s = new TopDownSprite(QPixmap(":/resources/graphics/misc/healthpotion.png"));
    setSprite(s);
    setDescription("A basic health potion. Heals health when used.");
}

/// Executed when the item is used.
/// Will heal owner.
void ItemHealthPotion::use_()
{
    Entity* o = owner();

    // do nothing if item has no owner
    if (o == nullptr)
        return;

    // otherwise, heal him
    o->setHealth(o->health() + amountToHealBy_);
}
