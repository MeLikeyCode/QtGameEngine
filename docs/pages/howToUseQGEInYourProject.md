How to use QGE in your project
==============================

Simplest way
------------

Simplest way is to:
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

More flexable way
-----------------
You can put the `qge` folder anywhere you want. You just have to properly `include()` the `qge.pri` file into your own `.pro` file.

For example, if i put my `qge` folder 2 directories above the directory that my `.pro` file is in, this is the `include()` i'd need to put in my `.pro` file:
~~~~
include(../../qge/qge.pri)
~~~~

Now, when you `#include` qge header files, you either type the relative link to them (e.g. `#include "../../qge/Entity.h"`) or add the folder that the `qge` folder is in to your compiler's header search path. In other words, add the following to your `.pro` file:
~~~
INCLUDEPATH += ../../ # add the directory 2 directories above to our include path
~~~
After adding the above include path, you can include qge headers like so: `#include "qge/Entity.h"`