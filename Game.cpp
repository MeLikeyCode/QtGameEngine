#include "Game.h"
#include "Map.h"
#include <QMouseEvent>
#include <QDebug> // TODO remove, testing purpose only
#include "Spear.h" // TODO remove, test

/// Creates an instance of the Game with some default options.
///
/// Creates a default Map, Camera, etc...
Game::Game(Map *map){
    map_ = map;
    player_ = nullptr;
    map->setGame(this);
    setMouseTracking(true);

    setScene(map->scene());
    setFixedSize(1024,768); // TODO REMOVE HARD CODED NUMBER
    setSceneRect(0,0,1024,768);

    // disable QGraphicsView's scroll bars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // TODO delete, test
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(askEnemiesToMove()));
    timer->start(4000);
}

/// Launches the Game.
void Game::launch(){
    showNormal();
}

/// Sets the Map.
void Game::setMap(Map *map){
    map_ = map;
}

/// Returns (a pointer to) the Game's Map.
Map *Game::map(){
    return map_;
}

void Game::mousePressEvent(QMouseEvent *event){
    // =TODO test code, remove=

    // move to clicked pos
    if (event->button() == Qt::LeftButton){
        // move player
        player_->moveTo(event->pos());
    }

//    // spear thrust
//    if (event->button() == Qt::LeftButton){
//        player_->spear_->attackThrust();
//    }

    // add rock (block cells at position)
    if (event->button() == Qt::RightButton){
        // add rock
        QPixmap pic(":resources/graphics/terrain/rock.png");
        QGraphicsPixmapItem* picI = new QGraphicsPixmapItem(pic);
        picI->setPos(event->pos().x()/64 * 64,event->pos().y()/64 * 64);
        map_->scene()->addItem(picI);

        map_->pathingMap().fill(event->pos());
        map_->drawPathingMap();
    }

//    // thrust right spear
//    if (event->button() == Qt::RightButton){
//        player_->spear2_->attackThrust();
//    }

}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    player()->rotateTo(event->pos());
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()){
        keysPressed_.insert(event->key());
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()){
        keysPressed_.erase(event->key());
    }
}

std::set<int> Game::keysPressed()
{
    return keysPressed_;
}

/// Sets the Entity controlled by the Player via keyboard and mouse.
void Game::setPlayer(Entity *player){
    player_ = player;
    player_->setCanMoveWithKeys(true);
}

Entity *Game::player(){
    return player_;
}

void Game::askEnemiesToMove()
{
    for (Entity* e:enemies_){
        if (e){
            e->moveTo(player_->pointPos());
        }
    }
}
