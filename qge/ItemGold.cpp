#include "ItemGold.h"

#include "Sprite.h"
#include "TopDownSprite.h"

using namespace qge;

ItemGold::ItemGold()
{
    // set sprite
    TopDownSprite* sprite = new TopDownSprite(QPixmap(":/resources/graphics/misc/gold.png"));
    setSprite(sprite);
    setDescription("A small amount of gold. Can be used as currency.");
}

void ItemGold::use_()
{
    // does nothing
}
