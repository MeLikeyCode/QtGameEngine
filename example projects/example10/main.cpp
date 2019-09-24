// Handling Input (keyboard + mouse)

// Hello again and welcome back to another QGE tutorial!
//
// Today, I will show you how to respond to user input (mouse/keyboard input).
//
// When any mouse or keyboard events occur, the Game class will let you know via one of its signals.
//
// Enjoy!
// - Abdullah

#include <QApplication>
#include <QDebug>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"

#include "MyQObject.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create MapGrid, Map, and Game
    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    qge::Map* map = new qge::Map();
    mapGrid->setMapAtPos(map,0,0);

    qge::Game* game = new qge::Game(mapGrid,0,0);
    game->launch();

    // connect to Game's event signals
    MyQObject* myQObject = new MyQObject();
    QObject::connect(game,&qge::Game::keyPressed,myQObject,&MyQObject::myKeyPressedCallback); // listen for when a key is pressed
    QObject::connect(game,&qge::Game::keyReleased,myQObject,&MyQObject::myKeyReleasedCallback); // listen for when a key is released
    // ... and so on

    // Now, run this example, and press some keys (try the escape key)

    return a.exec();
}
