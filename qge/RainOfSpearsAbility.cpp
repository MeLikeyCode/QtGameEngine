#include "RainOfSpearsAbility.h"

#include "Sprite.h"
#include "Sound.h"
#include "SpearProjectile.h"
#include "CBDamage.h"
#include "Map.h"

using namespace qge;


RainOfSpearsAbility::RainOfSpearsAbility(Entity* owner):
    NoTargetAbility(owner)
{
    setIcon(QPixmap(":/resources/graphics/weapons/trippleSpear.png"));
    setDescription("Rains spears around the owner. The spears damage enemies of the owner.");
    timer_ = new QTimer(this);
    soundEffect_ = new Sound("qrc:/resources/sounds/specialMove.wav",this);
}

void qge::RainOfSpearsAbility::useImplementation()
{
    connect(timer_,&QTimer::timeout,this,&RainOfSpearsAbility::spearStep_);
    timer_->start(500);
    soundEffect_->play(1);

    times_ = 0;
}

void RainOfSpearsAbility::spearStep_()
{
    // parameters
    const int NUM_WAVES = 15;
    const int NUM_SPEARS_TO_GENERATE_PER_WAVE = 5;
    const int X_RADIUS = 1000; // number of pixels around the owner's x position to spawn
                               // projectiles at
    const int Y_OFFSET = 1000; // number of pixels ABOVE the owner to spawn projectiles
    const int Y_RADIUS = 600;  // the y value "thickness" of possible y locations projectiles can spawn

    for (int i = 0, n = NUM_SPEARS_TO_GENERATE_PER_WAVE; i < n; i++){
        // get a random position some where above the entity.
        int randomXoffset = (rand() % X_RADIUS) - (X_RADIUS/2);
        int randomYoffset = rand() % Y_RADIUS - Y_OFFSET;
        int posX = owner()->pos().x() + randomXoffset;
        int posY = owner()->pos().y() + randomYoffset;
        QPointF randomPos(posX,posY);

        // get target position for the spear
        QPointF targetPos = randomPos;
        targetPos.setY(targetPos.y() + 20);
        targetPos.setX(targetPos.x());

        // create spear projectile
        SpearProjectile* spearProjectile = new SpearProjectile(800,5);
        spearProjectile->addEntityToNotCollideWith(owner()); // do not collide with owner of ability

        owner()->map()->addEntity(spearProjectile);

        spearProjectile->setPos(randomPos);
        spearProjectile->shootTowards(targetPos);
    }

    times_++;

    if (times_ >= NUM_WAVES){
        timer_->disconnect();
    }
}
