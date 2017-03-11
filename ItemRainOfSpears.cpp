#include "ItemRainOfSpears.h"
#include "Sprite.h"
#include <QPixmap>
#include "SpearProjectile.h"
#include <cassert>
#include "Entity.h"
#include "Inventory.h"
#include "stdlib.h" // TODO: use C++ random number generatio instead
#include "Sound.h"
#include "Map.h"
#include "CBDamage.h"

ItemRainOfSpears::ItemRainOfSpears()
{
    // set sprite
    Sprite* sprite = new Sprite(QPixmap(":/resources/graphics/weapons/trippleSpear.png"));
    setSprite(sprite);

    timer_ = new QTimer(this);
    soundEffect_ = new Sound("qrc:/resources/sounds/specialMove.wav",this);
}

/// Executed when the ItemRainOfSpears is asked to be used. Will generate a
/// bunch of falling spears.
void ItemRainOfSpears::use()
{
    // make sure the item is in an inventory
    assert(inventory() != nullptr);

    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    connect(timer_,&QTimer::timeout,this,&ItemRainOfSpears::spearStep_);
    timer_->start(500);
    soundEffect_->play(1);

    times_ = 0;
}

/// Executed every time ItemRainOfSpears is asked to generate a wave of spears.
void ItemRainOfSpears::spearStep_()
{
    // make sure the inventory has an owner
    Entity* owner = inventory()->entity();
    assert(owner != nullptr);

    const int NUM_WAVES = 15;
    const int NUM_SPEARS_TO_GENERATE = 5;
    const int X_RADIUS = 1000; // number of pixels around the owner's x position to spawn
                               // projectiles at
    const int Y_OFFSET = 1000; // number of pixels ABOVE the owner to spawn projectiles
    const int Y_RADIUS = 600;  // the y value "thickness" of possible y locations projectiles can spawn

    for (int i = 0, n = NUM_SPEARS_TO_GENERATE; i < n; i++){
        // get a random position some where above the entity.
        int randomXoffset = (rand() % X_RADIUS) - (X_RADIUS/2);
        int randomYoffset = rand() % Y_RADIUS - Y_OFFSET;
        int posX = owner->pointPos().x() + randomXoffset;
        int posY = owner->pointPos().y() + randomYoffset;
        QPointF randomPos(posX,posY);

        // get target position for the spear
        QPointF targetPos = randomPos;
        targetPos.setY(targetPos.y() + 20);
        targetPos.setX(targetPos.x());

        // create spear projectile
        SpearProjectile* spearProjectile = new SpearProjectile(800,5);
        CBDamage* cb = static_cast<CBDamage*>(spearProjectile->collisionBehavior());
        cb->addCollisionToIgnore(owner,spearProjectile); // don't damage owner
        owner->map()->addEntity(spearProjectile);
        spearProjectile->setPointPos(randomPos);
        spearProjectile->shootTowards(targetPos);
    }

    times_++;

    if (times_ >= NUM_WAVES){
        timer_->disconnect();
    }
}
