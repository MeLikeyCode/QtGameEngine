#include "Game.h"
#include "Map.h"
#include <QMouseEvent>
#include <QDebug> // TODO remove, testing purpose only
#include "Spear.h" // TODO remove, test
#include "DynamicEntity.h"
#include "Entity.h"
#include "Slot.h"
#include "StraightProjectile.h"
#include "HomingProjectile.h"

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

//    // weapon 1 attack
//    if (event->button() == Qt::LeftButton){
//        player_->slot("leftHand")->use();
//    }

//    // weapon 2 attack
//    if (event->button() == Qt::RightButton){
//        player_->slot("rightHand")->use();
//    }

//    // shoot straight projectile
//    if (event->button() == Qt::RightButton){

//        Slot* slot = player_->slot("leftHandRanged");
//        slot->use();

//    }

    // add default entity
    if (event->button() == Qt::LeftButton){
        Entity* e = new Entity();
        map()->addEntity(e);
        e->setPointPos(event->pos());
    }

    if (event->button() == Qt::RightButton){
        qDebug() << player_->entitiesInView().size();
    }

//    // spawn homing projectile towards player_
//    if (event->button() == Qt::LeftButton){
//        HomingProjectile* p = new HomingProjectile(player_);
//        p->setPointPos(QPointF(600,600));
//        map()->addEntity(p);
//        p->start();
//    }

//    // add rock (block cells at position)
//    if (event->button() == Qt::RightButton){
//        // add rock
//        QPixmap pic(":resources/graphics/terrain/rock.png");
//        QGraphicsPixmapItem* picI = new QGraphicsPixmapItem(pic);
//        picI->setPos(event->pos().x()/64 * 64,event->pos().y()/64 * 64);
//        map_->scene()->addItem(picI);

//        map_->pathingMap().fill(event->pos());
//        map_->drawPathingMap();
//    }

//    // thrust right spear
//    if (event->button() == Qt::RightButton){
//        player_->spear2_->attackThrust();
//    }

}

void Game::mouseMoveEvent(QMouseEvent *event)
{

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

/// Converts the specified point from Game coordinates to Map coordinates.
QPointF Game::mapToMap(const QPoint& point)
{
    return mapToScene(point);
}

/// Returns the keys that are _currently_ pressed.
std::set<int> Game::keysPressed()
{
    return keysPressed_;
}

/// Returns the current mouse position in Game coordinates.
QPoint Game::getMousePos()
{
    return mapFromGlobal(cursor().pos());
}

/// Sets the Entity controlled by the Player via keyboard and mouse.
void Game::setPlayer(DynamicEntity *player){
    player_ = player;
}

DynamicEntity *Game::player(){
    return player_;
}

void Game::askEnemiesToMove()
{
    for (DynamicEntity* e:enemies_){
        if (e){
            e->moveTo(player_->pointPos());
        }
    }
}
