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

    // create a temporary poof sprite at position
    Entity* poof = new Entity();
    Sprite* poofSprite = new Sprite();
    poofSprite->addFrames(":resources/graphics/effects/poof",10,"poof");
    poof->setSprite(poofSprite);
    poof->setPointPos(owner->pointPos());
    owner->map()->addEntity(poof);
    poofSprite->play("poof",1,40);

    // connect timer to delete the sprite
    QTimer* t = new QTimer();
    connect(t,SIGNAL(timeout()),poof,SLOT(deleteLater()));
    t->setSingleShot(true);
    t->start(300);


}
