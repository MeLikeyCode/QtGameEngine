How to use QGE in your project
==============================

1. copy entire `qge` folder to the same directory as your `.pro` file
2. add `include(qge/qge.pri)` in your `.pro` file

That's all folks! Now you can simply `#include <qge/SomeHeader.h>` to use QGE classes.

The following creates a minimalistic app that simply shows a blank Map.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
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
    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    mapGrid->setMapAtPos(map,0,0); // add map to map grid

    // create game
    qge::Game* game = new qge::Game(mapGrid,0,0);

    game->launch(); // launch game

    return a.exec();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
