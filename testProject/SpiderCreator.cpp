#include "SpiderCreator.h"

#include "qge/Entity.h"
#include "qge/AngledSprite.h"
#include "qge/SpriteSheet.h"
#include "qge/ECBodyThruster.h"
#include "qge/CItemDropper.h"
#include "qge/Game.h"

SpiderCreator::SpiderCreator():
    itemDropper_(new qge::CItemDropper())
{
    // empty
}

qge::Entity *SpiderCreator::createEntity()
{
    // create entity here
    qge::Entity* entity = new qge::Entity();
    entity->setGroup(0);
    qge::Game::game->diplomacyManager().setRelationship(entity->group(),1,qge::Relationship::ENEMY);
    entity->setOrigin(QPointF(64,64));

    qge::AngledSprite* spiderSprite = new qge::AngledSprite();
    entity->setSprite(spiderSprite);
    qge::SpriteSheet spiderSpriteSheet(":/resources/graphics/characterSpritesheets/spider.png",48,8,128,128);
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        spiderSprite->addFrames((180+45*i) % 360,"stand",spiderSpriteSheet,qge::Node(0,0+i),qge::Node(3,0+i));
        for (int j = 2; j > 0; --j){
            spiderSprite->addFrame(spiderSpriteSheet.tileAt(qge::Node(j,0+i)),"stand",(180+45*i) % 360);
        }
        // walk
        spiderSprite->addFrames((180+45*i) % 360,"walk",spiderSpriteSheet,qge::Node(4,0+i),qge::Node(11,0+i));

        // attack
        spiderSprite->addFrames((180+45*i) % 360,"attack",spiderSpriteSheet,qge::Node(12,0+i),qge::Node(15,0+i));

        // hit
        spiderSprite->addFrames((180+45*i) % 360,"hit",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(20,0+i));

        // die
        spiderSprite->addFrames((180+45*i) % 360,"die",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(23,0+i));

        // dieTwo
        spiderSprite->addFrames((180+45*i) % 360,"dieTwo",spiderSpriteSheet,qge::Node(24,0+i),qge::Node(31,0+i));
    }
    spiderSprite->play("stand",-1,10,0);

    qge::ECBodyThruster* bt = new qge::ECBodyThruster(entity);
    bt->setThrustDistance(100);

    entity->addSound("die","qrc:/resources/sounds/spiderDie.mp3");

    itemDropper_->addEntity(entity);

    return entity;
}
