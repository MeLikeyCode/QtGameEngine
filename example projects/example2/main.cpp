// Basic entity

// In this example, I show you how to create a fancy looking entity and place him in the map.
//
// We cover:
// - how to create an entity
// - how to create animations for an entity based on a sprite sheet
// - how to play a certain animation of an entity
//
// Please note: This example may look a bit daunting because there is so much text, but for the most
// part the text simply restates what is in the documentation for each element in the first place.
// In other words, I am being redundant. This example isn't as hard as it seems. Try to play around
// and modify it. Also, try to use qge::TopDownSprite instead of qge::AngledSprite and see
// how much easier that is.
//
// Don't worry, it's easy!
//
// Enjoy!
// - Abdullah

#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"
#include "qge/Entity.h"
#include "qge/SpriteSheet.h"
#include "qge/AngledSprite.h"
#include "qge/Node.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create map
    qge::Map* map = new qge::Map();

    // create map grid
    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    mapGrid->setMapAtPos(map,0,0); // add map to map grid

    // create game
    qge::Game* game = new qge::Game(mapGrid,0,0);

    // create an entity
    qge::Entity* entity = new qge::Entity();

    // == HERE IS THE NEW CODE FOR EXAMPLE 2 ==

    // let's create a visual represenation for the entity

    // first, we create a SpriteSheet, which allows us to easily extract frames from an actual sprite sheet
    qge::SpriteSheet minitaurSpriteSheet(":/resources/graphics/characterSpritesheets/minotaur_alpha.png", // location of sprite sheet
                                         24,    // number of frames horizontally in the sprite sheet
                                         8,     // number of frames vertically in the sprite sheet
                                         128,   // width of each frame in the sprite sheet
                                         128);  // height of each frame in the sprite sheet

    // for entities that have top down graphics we'd use qge::TopDownSprite, but we will get fancier
    // and make this entity have angled (e.g. isometric) graphics
    qge::AngledSprite* entitySprite = new qge::AngledSprite();

    // the sprite sheet has a bunch of different animations (e.g. walk, attack) each at 8 angles.
    // We will get the frames for the animations "stand", "walk" and "attack"
    for (int i = 0, n = 8; i < n; ++i){ // for each angle
        // stand
        entitySprite->addFrames((180+45*i) % 360,       // the angle of the entity in these frames
                                "stand",                // the name of the animation these frames are for
                                minitaurSpriteSheet,    // the sprite sheet we are extracting the frames from
                                qge::Node(0,0+i),       // the starting location in the sprite sheet
                                qge::Node(3,0+i));      // the ending location in the sprite sheet
        // above call, will extract all frames from the starting to the ending location
        // in the sprite sheet, add them to the entitySprite, and give them the animation name "stand"

        // you can ignore this block, but if you are curious, we add the last 2 frames in reverse order
        // again (this makes this particular animation look better!)
        for (int j = 2; j > 0; --j){
            entitySprite->addFrame(minitaurSpriteSheet.tileAt(qge::Node(j,0+i)),"stand",(180+45*i) % 360);
        }

        // walk - we extract frames in the sprite sheet for walking animation
        entitySprite->addFrames((180+45*i) % 360,"walk",minitaurSpriteSheet,qge::Node(4,0+i),qge::Node(11,0+i));

        // attack - we extract frames in the sprite sheet for attacking animation
        entitySprite->addFrames((180+45*i) % 360,"attack",minitaurSpriteSheet,qge::Node(12,0+i),qge::Node(18,0+i));
    }

    entity->setOrigin(QPointF(64,64)); // we will make point (64,64) (relative to top left of sprite) the point about which the entity will rotate
    entity->setSprite(entitySprite);   // FINALLY, let's assign the sprite to the entity

    // let's add the entity into the map at position (300,300)
    entity->setPos(QPointF(300,300));
    map->addEntity(entity);

    // let's play the stand animation of the entity
    entity->sprite()->play("stand" // name of the animation
                           ,-1     // the number of times to play the animation (-1 means play an infinite number of times)
                           ,10     // the fps (frames per second) to play at
                           ,0);    // the frame to start playing at


    game->launch(); // launch game
    return a.exec();
}
