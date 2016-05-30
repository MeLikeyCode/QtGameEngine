#include "ItemRainOfSpears.h"
#include "Sprite.h"
#include <QPixmap>
#include "SpearProjectile.h"
#include <cassert>
#include "DynamicEntity.h"
#include "Inventory.h"
#include "stdlib.h" // TODO: use C++ random number generatio instead

ItemRainOfSpears::ItemRainOfSpears()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":/resources/graphics/weapons/trippleSpear.png"));
    setSprite(sprite);
}

/// Executed when the ItemRainOfSpears is asked to be used. Will generate a
/// bunch of falling spears.
void ItemRainOfSpears::use()
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    assert(inventory()->entity() != nullptr);

    DynamicEntity* owner = inventory()->entity();

    int NUM_SPEARS_TO_GENERATE = 15;
    for (int i = 0, n = NUM_SPEARS_TO_GENERATE; i < n; i++){
        // get a random position some where above the entity.
        int randomXoffset = rand() % 1200 - 600;
        int randomYoffset = rand() % 1200 - 600;
        int posX = owner->pointPos().x() + randomXoffset;
        int posY = owner->pointPos().y() + randomYoffset;
        QPointF randomPos(posX,posY);

        // spawn projectile
        QPointF targetPos = randomPos;
        targetPos.setY(targetPos.y() + 20);
        targetPos.setX(targetPos.x() + 10);
        std::unordered_set<Entity*> noDmgList;
        noDmgList.insert(owner);
        SpearProjectile* spearProjectile = new SpearProjectile(randomPos,
                                                               targetPos,
                                                               600,
                                                               5,
                                                               noDmgList,
                                                               owner->map());
        spearProjectile->startMoving();
    }
}
