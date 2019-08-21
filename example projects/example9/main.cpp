// Basic GUI

// Hello again and welcome back to another QGE tutorial!
//
// Today, I will show you how to create a basic GUI.
//
// Enjoy!
// - Abdullah

#include <QApplication>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    qge::Map* map = new qge::Map();
    mapGrid->setMapAtPos(map,0,0);

    qge::Game* game = new qge::Game(mapGrid,0,0);
    game->launch();

    return a.exec();
}
