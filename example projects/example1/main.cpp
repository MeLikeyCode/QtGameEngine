// Basic map

// In this example, we create a blank map and launch the game!
// The purpose of this example is to get you warmed up, to get you up and running!
// Enjoy!
// - Abdullah

#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create map
    qge::Map* map = new qge::Map();

    // create map grid
    // All Maps go inside MapGrids.
    // It might be a good idea to quickly **skim** the documentation for classes Map and MapGrid.
    // You don't have to understand everything in the docs right now, but I believe skimming will help you out!
    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    mapGrid->setMapAtPos(map,0,0); // add map to map grid

    // create game
    // You can think of the game as the window really...
    qge::Game* game = new qge::Game(mapGrid,0,0);

    game->launch(); // launch game (basically shows the window)

    return a.exec();
}
