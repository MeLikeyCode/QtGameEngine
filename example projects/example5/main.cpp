// Enemy (body thrusting)
//
// Hello and welcome back!
// In this example, I'll show you how to create an enemy that will "body thrust" (think: body slam) your player!


#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"
#include "qge/Entity.h"
#include "qge/Utilities.h"
#include "qge/EntitySprite.h"

#include "qge/ECKeyboardMoverPerspective.h"
#include "qge/ECMouseFacer.h"
#include "qge/PathingMap.h"
#include "qge/ECCameraFollower.h"
#include "qge/ECMapMover.h"
#include "qge/ECCurrentMapGrabber.h"

#include "qge/SpriteSheet.h"
#include "qge/AngledSprite.h"
#include "qge/ECBodyThruster.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create map
    qge::Map* map = new qge::Map();

    // create map grid
    qge::MapGrid* mapGrid = new qge::MapGrid(1,2);
    mapGrid->setMapAtPos(map,0,1); // add map to map grid

    // create game
    qge::Game* game = new qge::Game(mapGrid,0,1);

    // create minitaur looking entity
    qge::Entity* entity = qge::getMinitaurEntity();

    // add the entity into the map at position (300,300)
    entity->setPos(QPointF(300,300));
    map->addEntity(entity);

    // play the stand animation of the entity
    entity->sprite()->play("stand" ,-1 ,10 ,0);

    // make entity move relative to its own facing angle in reponse to the arrow keys
    qge::ECKeyboardMoverPerspective* keyboardMoverController = new qge::ECKeyboardMoverPerspective(entity);

    // make entity always face the mouse
    qge::ECMouseFacer* mouseFacerController = new qge::ECMouseFacer(entity);

    // Let's add another map
    qge::Map* map2 = new qge::Map(50,50,32);
    mapGrid->setMapAtPos(map2,0,0);

    // let's make the camera follow the entity
    qge::ECCameraFollower* camFollowerCont = new qge::ECCameraFollower(entity);

    // let's make the the entity move to the next map when close enough to the border of the current map
    qge::ECMapMover* mapMoverCont = new qge::ECMapMover(entity);

    // let's set game's current map as the map that the entity is in
    qge::ECCurrentMapGrabber* mapGrabber = new qge::ECCurrentMapGrabber(entity);

    // == NEW STUFF ==

    // let's create a new entity that will look like a spider

    qge::Entity* spiderEntity = new qge::Entity();

    qge::SpriteSheet spiderSpriteSheet(":/resources/graphics/characterSpritesheets/spider.png",48,8,128,128);

    qge::AngledSprite* spiderSprite = new qge::AngledSprite();
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

        // hit (animation that will play when the spider is hit - but not killed)
        spiderSprite->addFrames((180+45*i) % 360,"hit",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(20,0+i));

        // die (animation that will play when spider dies)
        spiderSprite->addFrames((180+45*i) % 360,"die",spiderSpriteSheet,qge::Node(18,0+i),qge::Node(23,0+i));

        // dieTwo (a second animation that might play when spider dies - just adds variety)
        spiderSprite->addFrames((180+45*i) % 360,"dieTwo",spiderSpriteSheet,qge::Node(24,0+i),qge::Node(31,0+i));
    }

    spiderEntity->setSprite(spiderSprite);
    spiderEntity->setOrigin(QPointF(64,64)); // make position (64,64), relative to the top left of the sprite, be the point about which the entity rotates and stuff
    spiderEntity->sprite()->play("stand",-1,15,0);
    spiderEntity->setPos(QPointF(600,600));
    map->addEntity(spiderEntity);

    // let's make the entity enemies with the minitaur
    entity->setGroup(0);
    spiderEntity->setGroup(1);
    spiderEntity->addEnemyGroup(0);

    // let's make the entity body thrust (or body slam - w.e. you wanna call it) enemies that come to inside it's field of view (i.e. in front of it)
    qge::ECBodyThruster* bodyThrusterC = new qge::ECBodyThruster(spiderEntity);

    // let's show the field of view of the body thruster
    bodyThrusterC->setShowFOV(true);

    // Run the game and test it out! Get in the field of view of the spider and watch him chase you down! Once he's close enough, he'll thrust at you, damaging you.
    // Set the health of your mintaur to 100 or something like that so that you don't die as fast!
    entity->setHealth(100);

    game->launch(); // launch game
    return a.exec();
}
