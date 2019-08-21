// Basic GUI

// Hello again and welcome back to another QGE tutorial!
//
// Today, I will show you how to create a basic GUI.
//
// All Gui objects inherit from qge::Gui. They can be added to a Game (via Game::addGui()) or to a Map (via Map::addGui()).
// If they are added to a Game, they will be positioned relative to the top left of the screen.
// If they are added to a Map, they will be positioned relative to the top left of the Map.
// All Guis that you add to a Game, will always show on the screen, regardless of what Map you are currently seeing.
// For Guis that you placed to a Map, they are only seen when you are seeing that map (i.e. when that map is the current map).
//
// Enjoy!
// - Abdullah

#include <QApplication>
#include <QDebug>

#include "qge/MapGrid.h"
#include "qge/Map.h"
#include "qge/Game.h"

#include "qge/Label.h"
#include "qge/Button.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // create MapGrid, Map, and Game
    qge::MapGrid* mapGrid = new qge::MapGrid(1,1);
    qge::Map* map = new qge::Map();
    mapGrid->setMapAtPos(map,0,0);

    qge::Game* game = new qge::Game(mapGrid,0,0);
    game->launch();

    // create some Guis

    // create a Label
    qge::Label* label = new qge::Label("Hello! I'm a label!");
    label->setGuiPos(QPointF(50,50));
    game->addGui(label); // add the Label to the *Game*

    // create a Button
    qge::Button* button = new qge::Button();
    button->setText("Hello, I'm a button! Click me!");
    button->setGuiPos(QPointF(100,100));
    map->addGui(button); // add the Button to the *Map*

    // Button has a "clicked" signal that is emitted whenever it is clicked.
    // But for convenience, you can also have it execute a function each time it is clicked.
    // Here we have it execute a lambda function each time it is clicked.
    auto cb = [](qge::Button* btn){
      qDebug() << "Ouch! You clicked me!";
    };
    button->setOnClickCB(cb);

    return a.exec();
}

// So let's recap!
// - we added a Label Gui to the Game (thus it is positioned relative to the top left of the *screen* (and it will be visible no matter what Map we are currently seeing).
// - we added a Button Gui to the Map (thus it is positioned relative to the top left of the *map*
// - we had a function be called each time the Button is clicked
//
// That is all for today!
