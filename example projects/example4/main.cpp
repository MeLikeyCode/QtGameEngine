// Multiple maps
//
// Hello and welcome back!
//
// In this example, I'll show you how to place multiple maps in your game, and have your entity
// move between them.
//
// Hope you enjoy it!

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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create map
    qge::Map* map = new qge::Map();

    // create map grid
    qge::MapGrid* mapGrid = new qge::MapGrid(1,2); // HEY! PAY ATTENTION HERE! We change the size of the map grid so we can put 2 maps stacked vertically
    mapGrid->setMapAtPos(map,0,1); // add map to map grid

    // create game
    qge::Game* game = new qge::Game(mapGrid,0,1);

    // create minitaur looking entity (we did this in example 2, I just moved all that code to a function called qge::getMinitaurEntity())
    qge::Entity* entity = qge::getMinitaurEntity();

    // add the entity into the map at position (300,300)
    entity->setPos(QPointF(300,300));
    entity->setFacingAngle(0);
    map->addEntity(entity);

    // play the stand animation of the entity
    entity->sprite()->play("stand" ,-1 ,10 ,0);

    // make entity move relative to its own facing angle in reponse to the arrow keys
    qge::ECKeyboardMoverPerspective* keyboardMoverController = new qge::ECKeyboardMoverPerspective(entity);

    // make entity always face the mouse
    qge::ECMouseFacer* mouseFacerController = new qge::ECMouseFacer(entity);

    // == NEW STUFF (we did all of the above in previous examples) ==

    // Let's add another map
    qge::Map* map2 = new qge::Map(50,50,32);
    mapGrid->setMapAtPos(map2,0,0);

    // let's make the camera follow the entity
    qge::ECCameraFollower* camFollowerCont = new qge::ECCameraFollower(entity);

    // let's make the the entity move to the next map when close enough to the border of the current map
    qge::ECMapMover* mapMoverCont = new qge::ECMapMover(entity);

    // let's set game's current map as the map that the entity is in
    qge::ECCurrentMapGrabber* mapGrabber = new qge::ECCurrentMapGrabber(entity);

    // That is all for this tutorial! Play around with this, read/skim some of the documentation for these classes,
    // and then move on to the next example when you're ready!
    // Hope that was kinda fun for you!

    game->launch(); // launch game
    return a.exec();
}
