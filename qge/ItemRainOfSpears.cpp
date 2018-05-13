#include "ItemRainOfSpears.h"

#include <QPixmap>
#include <cassert>

#include "TopDownSprite.h"
#include "Inventory.h"
#include "RainOfSpearsAbility.h"

using namespace qge;

ItemRainOfSpears::ItemRainOfSpears()
{
    setSprite(new TopDownSprite(QPixmap(":/resources/graphics/weapons/trippleSpear.png")));
    setDescription("Rains spears in the vicinity. The spears damage enemies.");
    ability_ = new RainOfSpearsAbility();
}

/// Executed when the ItemRainOfSpears is asked to be used. Will generate a
/// bunch of falling spears.
void ItemRainOfSpears::use_()
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    ability_->setOwner(inventory()->entity());
    ability_->use();
}
