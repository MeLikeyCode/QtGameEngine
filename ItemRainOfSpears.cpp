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

    int NUM_SPEARS_TO_GENERATE = 5;
    int X_OFFSET_RANGE = 1000; // number of pixels around the owner's x position to spawn
                               // projectiles at
    int Y_OFFSET = 1000; // number of pixels ABOVE the owner to spawn projectiles
    int Y_OFFSET_RANGE = 600; // the y value "thickness" of possible y locations projectiles can spawn
    for (int i = 0, n = NUM_SPEARS_TO_GENERATE; i < n; i++){
        // get a random position some where above the entity.
        int randomXoffset = (rand() % X_OFFSET_RANGE) - (X_OFFSET_RANGE/2);
        int randomYoffset = rand() % Y_OFFSET_RANGE - Y_OFFSET;
        int posX = owner->pointPos().x() + randomXoffset;
        int posY = owner->pointPos().y() + randomYoffset;
        QPointF randomPos(posX,posY);

        // spawn projectile
        QPointF targetPos = randomPos;
        targetPos.setY(targetPos.y() + 20);
        targetPos.setX(targetPos.x());
        std::unordered_set<Entity*> noDmgList;
        noDmgList.insert(owner);
        noDmgList.insert(this);
        SpearProjectile* spearProjectile = new SpearProjectile(800,5,noDmgList);
        owner->map()->addEntity(spearProjectile);
        spearProjectile->setPointPos(randomPos);
        spearProjectile->addToNoDamageList(owner);
        spearProjectile->shootTowards(targetPos);
    }

    times_++;

    if (times_ > 15){
        timer_->disconnect();
    }
}
