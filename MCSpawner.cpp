#include "MCSpawner.h"

#include <QTimer>
#include <cassert>

#include "Entity.h"
#include "Map.h"
#include "AngledSprite.h"
#include "ECBodyThruster.h"
#include "SpriteSheet.h"
#include "CItemDropper.h"


extern CItemDropper* itemDropper;

MCSpawner::MCSpawner(Map *map, const QRectF &region, MCSpawner::SpawnType spawnType, int max, double numPerSec):
    map_(map),
    region_(region),
    spawnType_(spawnType),
    max_(max),
    ratePerSec_(numPerSec),
    timer_(new QTimer(this))
{
    connect(timer_,&QTimer::timeout,this,&MCSpawner::onTimeout_);
    turnOn();
}

void MCSpawner::turnOn()
{
    double delayS = 1.0 / ratePerSec_;
    double delayMS = delayS * 1000;
    timer_->start(delayMS);
}

void MCSpawner::turnOff()
{
    timer_->stop();
}

void MCSpawner::onTimeout_()
{
    // get random point in region
    QPointF randPoint = random_.randQPointF(region_);

    // create entity here
    Entity* entity = new Entity();
    entity->addEnemyGroup(1);
    if (spawnType_ == SpawnType::Spider){
        AngledSprite* spiderSprite = new AngledSprite();
        entity->setSprite(spiderSprite);
        SpriteSheet spiderSpriteSheet(":/resources/graphics/characterSpritesheets/spider.png",48,8,128,128);
        for (int i = 0, n = 8; i < n; ++i){ // for each angle
            // stand
            spiderSprite->addAnimation((180+45*i) % 360,"stand",spiderSpriteSheet,Node(0,0+i),Node(3,0+i));
            for (int j = 2; j > 0; --j){
                spiderSprite->addFrame(spiderSpriteSheet.tileAt(Node(j,0+i)),"stand",(180+45*i) % 360);
            }
            // walk
            spiderSprite->addAnimation((180+45*i) % 360,"walk",spiderSpriteSheet,Node(4,0+i),Node(11,0+i));

            // attack
            spiderSprite->addAnimation((180+45*i) % 360,"attack",spiderSpriteSheet,Node(12,0+i),Node(15,0+i));

            // hit
            spiderSprite->addAnimation((180+45*i) % 360,"hit",spiderSpriteSheet,Node(18,0+i),Node(20,0+i));

            // die
            spiderSprite->addAnimation((180+45*i) % 360,"die",spiderSpriteSheet,Node(18,0+i),Node(23,0+i));

            // dieTwo
            spiderSprite->addAnimation((180+45*i) % 360,"dieTwo",spiderSpriteSheet,Node(24,0+i),Node(31,0+i));
        }
        spiderSprite->play("stand",-1,10,0);
        spiderSprite->setOrigin(QPointF(64,64));

        ECBodyThruster* bt = new ECBodyThruster(entity);
        bt->setThrustDistance(100);

        entity->addSound("die","qrc:/resources/sounds/spiderDie.mp3");
    }

    // add entity to map
    map_->addEntity(entity);
    entity->setPos(randPoint);

    // add entity to item dropper
    itemDropper->addEntity(entity);
}
