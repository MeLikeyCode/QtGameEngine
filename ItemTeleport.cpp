#include "ItemTeleport.h"
#include "Sprite.h"
#include "Entity.h"
#include <cassert>
#include "Inventory.h"
#include "Map.h"
#include "Sound.h"

ItemTeleport::ItemTeleport()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":resources/graphics/misc/teleport.png"));
    setSprite(sprite);

    soundEffect_ = new Sound("qrc:/resources/sounds/blink.wav");
}

void ItemTeleport::use(const QPointF &point)
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    // play sound effect
    soundEffect_->play(1);

    // move owner to the specified location
    owner->setPointPos(point);

    // play a "poof" animation at the location of the owner
    Sprite* poofSprite = new Sprite();
    poofSprite->addFrames(":resources/graphics/effects/poof",10,"poof");
    owner->map()->playOnce(poofSprite,"poof",50,owner->pointPos());
}
