#include "Game.h"
#include "Map.h"
#include <QMouseEvent>
#include <QDebug> // TODO remove, testing purpose only
#include "Spear.h" // TODO remove, test
#include "DynamicEntity.h"
#include "Entity.h"
#include "Slot.h"
#include "ProjectileMoveBehaviorHoming.h"
#include "ProjectileCollisionBehaviorDamage.h"
#include "ProjectileRangeReachedBehaviorDestroy.h"
#include "Projectile.h"
#include "Enemy.h"
#include "WeaponSlot.h"
#include "Axe.h"
#include "Bow.h"

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

    // move test entities to target pos
    if (event->button() == Qt::LeftButton){
        for (DynamicEntity* entity:testEntities_){
            entity->moveTo(event->pos());
        }
    }

    // create a test entity and add it
    if (event->button() == Qt::RightButton){
        DynamicEntity* testE = new DynamicEntity();
        map_->addEntity(testE);
        testE->setPointPos(event->pos());
        testEntities_.push_back(testE);

        // give the entity a sprite (overrides default one)
        Sprite* spr = new Sprite();
        testE->setSprite(spr);
        spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
        spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
        spr->play("stand",1,1); // play stand anim
    }

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

//    // create enemy
//    if (event->button() == Qt::LeftButton){
//        // create enemy (will follow/attack its enemies)
//        Enemy* e = new Enemy();
//        e->setPointPos(event->pos());
//        map()->addEntity(e);


//    }

//    // add default entity
//    if (event->button() == Qt::LeftButton){
//        Entity* e = new Entity();
//        map()->addEntity(e);
//        e->setPointPos(event->pos());
//    }

      // display number of entities in view
//    if (event->button() == Qt::RightButton){
//        qDebug() << player_->entitiesInView().size();
//    }

//    // spawn homing projectile towards player_
//    if (event->button() == Qt::LeftButton){
//        ProjectileMoveBehaviorHoming* mb = new ProjectileMoveBehaviorHoming(player_);
//        ProjectileCollisionBehaviorDamage* cb = new ProjectileCollisionBehaviorDamage();
//        ProjectileRangeReachedBehaviorDestroy* rc = new ProjectileRangeReachedBehaviorDestroy();

//        Projectile* p = new Projectile(mb,cb,rc);
//        map()->addEntity(p);
//        p->go(QPointF(0,0),QPointF(400,400),1000);
//    }

    // add rock (block cells at position)
    if (event->button() == Qt::MiddleButton){
        // add rock
        QPixmap pic(":resources/graphics/terrain/rock.png");
        QGraphicsPixmapItem* picI = new QGraphicsPixmapItem(pic);
        picI->setPos(event->pos().x()/64 * 64,event->pos().y()/64 * 64);
        map_->scene()->addItem(picI);

        map_->pathingMap().fill(event->pos());
        map_->drawPathingMap();
    }

//    // mvoe player to target pos
//    if (event->button() == Qt::LeftButton){
//        player_->moveTo(event->pos());
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
