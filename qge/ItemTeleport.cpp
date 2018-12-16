#include "ItemTeleport.h"

#include "Sprite.h"
#include "Entity.h"
#include "Inventory.h"
#include "Map.h"
#include "Sound.h"
#include "TopDownSprite.h"

using namespace qge;

ItemTeleport::ItemTeleport()
{
    // set sprite
    TopDownSprite* sprite = new TopDownSprite(QPixmap(":resources/graphics/misc/teleport.png"));
    setSprite(sprite);
    setDescription("Allows user to teleport to a specified location within some distance.");
    soundEffect_ = new Sound("qrc:/resources/sounds/blink.wav", this);
}

void ItemTeleport::use_(const QPointF &point)
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    // play sound effect
    soundEffect_->play(1);

    // move owner to the specified location
    owner->setPos(point);

    // play a "poof" animation at the location of the owner
    Sprite* poofSprite = new Sprite();
    poofSprite->addFrames(":resources/graphics/effects/poof",10,"poof","poof");
    owner->map()->playOnce(poofSprite,"poof",50,owner->pos());
}
