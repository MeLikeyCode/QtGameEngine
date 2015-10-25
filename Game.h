#ifndef GAME_H
#define GAME_H

//inherits
#include <QGraphicsView>

// members
#include "Map.h"
#include <set>

/// Represents an instance of the game.
/// @author Abdullah Aghazadah
/// @date 6-3-15
///
/// A Game has a Map with a bunch of Entities in it. It also has a Camera to
/// view parts of the Map.
///
/// To create a Game:
/// Game* game = new Game();
///
/// To start the Game:
/// game->launch(); // a default Map/Camera will be generated
///
/// To set the Map of the game, use Game::setMap(Map*).
/// To set the Camera, use Game::setCamera(Camera*).
class Game: public QGraphicsView{
    Q_OBJECT //todo delete test
public:
    // constructor
    Game(Map* map);

    void launch();

    void setMap(Map* map);
    Map* map();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    std::set<int> keysPressed();

    void setPlayer(Entity* player);
    Entity* player();

    std::vector<Entity*> enemies_; // TODO delete test

public slots:
    void askEnemiesToMove(); // TODO delete test

private:
    // main private attributes
    Map* map_;
    std::set<int> keysPressed_;

    // TODO remove the following attributes, test attributes
    Entity* player_;


};

#endif // GAME_H
