#include "ItemGold.h"
#include "Sprite.h"

ItemGold::ItemGold()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":/resources/graphics/misc/gold.png"));
    setSprite(sprite);
}

void ItemGold::use()
{
    // does nothing
}
