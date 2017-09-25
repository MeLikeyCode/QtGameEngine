#include "ItemGold.h"
#include "Sprite.h"
#include "TopDownSprite.h"

ItemGold::ItemGold()
{
    // set sprite
    TopDownSprite* sprite = new TopDownSprite(QPixmap(":/resources/graphics/misc/gold.png"));
    setSprite(sprite);
}

void ItemGold::use()
{
    // does nothing
}
