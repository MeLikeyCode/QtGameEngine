// Moving/rotating entity

// This example picks up where example 2 left of. I took all of the code that created the entity and its graphics and
// put it into a function called getMinitaurEntity(). We will now take this entity and make it move in response
// to the keyboard keys being pressed. We will also make the entity always face the mouse position.
//
// Have fun with it! I Hope you enjoy this example!
// -Abdullah

#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"
#include "qge/Entity.h"
#include "qge/Utilities.h"
#include "qge/EntitySprite.h"

#include "qge/ECKeyboardMoverPerspective.h"
#include "qge/ECMouseFacer.h"


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

    // create minitaur looking entity (we did this in example 2, I just moved all that code to a function called qge::getMinitaurEntity())
    qge::Entity* entity = qge::getMinitaurEntity();

    // add the entity into the map at position (300,300)
    entity->setPos(QPointF(300,300));
    entity->setFacingAngle(0);
    map->addEntity(entity);

    // play the stand animation of the entity
    entity->sprite()->play("stand" ,-1 ,10 ,0);

    // == NEW STUFF (we've done all of the above code in previous tutorials) ==

    // Let's make the entity move when the arrow keys are pressed.
    // The way you usually add behaviors like this to an entity is by attaching "EntityControllers"
    // to the entity. There are lots of built in EntityControllers that add lots of different
    // types of behaviors to entities. It might be beneficial to look up all (or at least some) of the
    // built in EntityControllers.

    // All entity controllers inherit from the class EntityController.
    // You can use the type hierarchy feature of Qt Creator or inhertence diagram of the doxygen
    // generated documentation to find all the classes that inherit from EntityController.

    // Lucky for you, there is a pre built EntityController called ECKeyboardMoverPerspective,
    // which moves its controlled entity in response to the arrow keys! All we gotta do is attach
    // the controller.

    qge::ECKeyboardMoverPerspective* keyboardMoverController = new qge::ECKeyboardMoverPerspective(entity);

    // That is all! When you construct EntityControllers, you pass the controlled entity into the constructor.
    // The controller then starts doing its thaaaaang. Run the game right now and pressed the arrow keys and
    // notice that when you press up, the entity moves forward, when you press down it moves backward, etc.

    // Look at how much work the entity controller did for you! It listens to keyboard events and in response
    // it moves the entity in the correct direction (relative to its facing angle), it even asks the entity
    // to play a "walk" animation!

    // Now, we want to make the entity always face the mouse. You guessed it, a built in entity controller
    // exists for that!

    qge::ECMouseFacer* mouseFacerController = new qge::ECMouseFacer(entity);

    // Run the game again. Move around the map.
    // That is all for this tutorial, hope you found it informative and fun!

    // BONUS: Hello folks, I'm back! I have a few things you can experiment with to earn some bonus pats on the back!

    // The ECKeyboardMoverPerspective* moves the entity relative to its current facing direction. So when the entity is
    // facing angle 20 degrees and you press up, it will move forward at that angle. The ECKeyboardMover4Directional
    // controller moves the entity relative to the screen, not its facing direction.

    // BONUS OBJECTIVE: Try out ECKeyboardMover4Directional and ECKeyboardMover8Directional (also skim their documentation).

    game->launch(); // launch game
    return a.exec();
}
