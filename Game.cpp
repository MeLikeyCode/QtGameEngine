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
#include "Projectile.h"
#include "AIEntity.h"
#include "WeaponSlot.h"
#include "Axe.h"
#include "Bow.h"
#include "InventoryViewer.h"
#include <cstdlib>
#include <ctime>
#include "WeatherEffect.h"
#include "RainWeather.h"
#include "MapGrid.h"
#include "Gui.h"

/// Creates an instance of the Game with some default options.
///
/// Creates a default Map, Camera, etc...
Game::Game(MapGrid *mapGrid, int xPosOfStartingMap, int yPosOfStartingMap){
    mapGrid_ = mapGrid;
    player_ = nullptr;

    for (Map* map:mapGrid_->maps()){
        map->setGame(this);
    }
    setMouseTracking(true);

    currentMap_ = mapGrid_->mapAt(xPosOfStartingMap,yPosOfStartingMap);
    setScene(currentMap_->scene());

    // disable QGraphicsView's scroll bars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // TODO delete, test
    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(askEnemiesToMove()));
    timer->start(4000);

    updateTimer_ = new QTimer(this);
    connect(updateTimer_,&QTimer::timeout,this,&Game::updateGuiPositions);
    updateTimer_->start(0);

    setMouseMode(MouseMode::regular);
}

/// Launches the Game.
void Game::launch(){
    //showNormal(); // TODO: eventually should showFullscreen() (or parametrize to
                  // allow launching normal or full screen

    showFullScreen();
    setSceneRect(0,0,width(),height());
}

MapGrid *Game::mapGrid()
{
    return mapGrid_;
}

/// Sets the current Map that the Game is visualizing.
void Game::setCurrentMap(Map *map){
    Map* oldMap = currentMap_;
    currentMap_ = map;
    setScene(currentMap_->scene());
    setSceneRect(0,0,width(),height());  

    // move guis over
    for (Gui* gui:guis_){
        scene()->addItem(gui->graphicsItem_);
    }

    emit mapChanged(oldMap,map);
}

/// Returns the current Map.
Map *Game::currentMap(){
    return currentMap_;
}

/// Sets the center of the camear to be looking at the specified point on the
/// current Map.
void Game::setCenterCamPos(QPointF position)
{
    double camWidth = this->sceneRect().width();
    double camHeight = this->sceneRect().height();
    QPointF topLeft(position.x() - camWidth/2,position.y() - camHeight/2);
    this->setSceneRect(topLeft.x(),topLeft.y(),camWidth,camHeight);
}

/// Returns the pos of the center of the camera.
QPointF Game::centerCamPos()
{
    double x = sceneRect().topLeft().x() + sceneRect().width()/2;
    double y = sceneRect().topLeft().y() + sceneRect().height()/2;
    return QPointF(x,y);
}

/// Returns a rectangle representing the current position/size of the camera.
QRectF Game::cam()
{
    return sceneRect();
}

/// Moves the camera by the specified vector on the Map.
void Game::moveCam(QVector2D byVector)
{
    QPointF oldPoint(this->sceneRect().topLeft().x(),this->sceneRect().topLeft().y());
    QPointF newPoint(oldPoint.x() + byVector.x(),oldPoint.y() + byVector.y());
    double oldWidth = this->sceneRect().width();
    double oldHeight = this->sceneRect().height();
    this->setSceneRect(newPoint.x(), newPoint.y(), oldWidth,oldHeight);
}

/// Moves the camera up by the specified amount.
/// If byAmount is negative, then obviously, it will be moved down...
/// I can't believe I felt like I had to tell you that ^__^
void Game::moveCamUp(double byAmount)
{
    moveCam(QVector2D(0,-1 * byAmount));
}

/// Moves the camera down by the specified amount.
void Game::moveCamDown(double byAmount)
{
    moveCam(QVector2D(0,byAmount));
}

/// Moves the camera left by the specified amount.
void Game::moveCamLeft(double byAmount)
{
    moveCam(QVector2D(-1 * byAmount,0));
}

/// Moves the camera right by the specified amount.
void Game::moveCamRight(double byAmount)
{
    moveCam(QVector2D(byAmount,0));
}

void Game::mousePressEvent(QMouseEvent *event){
    // if in selectPosition MouseMode, emit positionSelected
    if (mouseMode_ == MouseMode::selectPosition){
        emit positionSelected(mapToMap(event->pos()));
        return;
    }

    // if in selectEntity MouseMode and there is an entity under click,
    // get that entity and emit entitySelected
    if (mouseMode_ == MouseMode::selectEntity){
        std::unordered_set<Entity*> entitiesUnderClick = currentMap()->entities(mapToMap(event->pos()));
        if (entitiesUnderClick.size() > 0){
            emit entitySelected(*entitiesUnderClick.begin());
            return;
        }
    }

    // if control reaches here, we are in regular MouseMode, just let the event
    // propogate to the correct QGraphicsItem, which will handle it
    // TODO: someday, set up your own event propogation system at the Entity level
    // as opposed to at the QGraphicsItem level

    // =TODO test code, remove=

//    // move test entities to target pos
//    if (event->button() == Qt::LeftButton){
//        for (DynamicEntity* entity:testEntities_){
//            entity->moveTo(event->pos());
//        }
//    }

//    // create a test entity and add it
//    if (event->button() == Qt::RightButton){
//        DynamicEntity* testE = new DynamicEntity();
//        map_->addEntity(testE);
//        testE->setPointPos(event->pos());
//        testEntities_.push_back(testE);

//        // give the entity a sprite (overrides default one)
//        Sprite* spr = new Sprite();
//        testE->setSprite(spr);
//        spr->addFrames(":resources/graphics/human",1,"stand"); // stand anim
//        spr->addFrames(":resources/graphics/human",6,"walk");  // walk anim
//        spr->play("stand",1,1); // play stand anim
//    }

//    // add rock (block cells at position)
//    if (event->button() == Qt::MiddleButton){
//        // add rock
////        QPixmap pic(":resources/graphics/terrain/rock.png");
////        QGraphicsPixmapItem* picI = new QGraphicsPixmapItem(pic);
////        picI->setPos(event->pos().x()/64 * 64,event->pos().y()/64 * 64);
////        map_->scene()->addItem(picI);

//        map_->pathingMap().fill(mapToMap(event->pos()));
//        map_->drawPathingMap();
//    }

    // weapon 1 attack
    if (event->button() == Qt::MiddleButton){
        for (Slot* slot:player_->getSlots()){
            if (slot->isFilled())
            slot->use();
        }
    }

//    // weapon 2 attack
//    if (event->button() == Qt::RightButton){
//        player_->slot("rightHand")->use();
//    }

//    // shoot straight projectile
//    if (event->button() == Qt::RightButton){

//        Slot* slot = player_->slot("leftHandRanged");
//        slot->use();

//    }

//    // create AIEntity (part of grp 1)
//    if (event->button() == Qt::RightButton){
//        // create enemy (will follow/attack its enemies)
//        AIEntity* e = new AIEntity();
//        e->setGroupID(1);
//        e->addEnemy(0);
//        e->setPointPos(mapToMap(event->pos()));
//        currentMap()->addEntity(e);
//    }

//    // create AIEntity (part of grp 2)
//    if (event->button() == Qt::RightButton){
//        // create enemy (will follow/attack its enemies)
//        AIEntity* e = new AIEntity();
//        e->setGroupID(2);
//        e->addEnemy(1);
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

//    // mvoe player to target pos
//    if (event->button() == Qt::LeftButton){
//        player_->moveTo(event->pos());
//    }

//    // thrust right spear
//    if (event->button() == Qt::RightButton){
//        player_->spear2_->attackThrust();
//    }

    QGraphicsView::mousePressEvent(event);
}

void Game::mouseMoveEvent(QMouseEvent *event)
{

}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        close();
    }

    if (!event->isAutoRepeat()){
        keysPressed_.insert(event->key());
    }

    // TODO: remove, test
    if (event->key() == Qt::Key_E){
        currentMap()->weatherEffect()->start();
    }
    if (event->key() == Qt::Key_R){
        currentMap()->weatherEffect()->stop();
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()){
        keysPressed_.erase(event->key());
    }
}

void Game::addGui(Gui *gui)
{
    guis_.insert(gui);
    scene()->addItem(gui->graphicsItem_);
    gui->graphicsItem_->setZValue(100); // put Guis on top
}

/// Converts the specified point from Game coordinates to Map coordinates (
/// of the current Map).
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

/// Returns the MouseMode that the Game is in.
Game::MouseMode Game::mouseMode()
{
    return mouseMode_;
}

/// Sets the MouseMode that the game should b in.
/// Regular - Mouse events go to Item clicked on, they can respond accordingly.
/// SelectPosition - Every click will trigger the PositionSelected signal
/// SelectEntity - When an Entity is clicked, will trigger the EntitySelected signal.
void Game::setMouseMode(Game::MouseMode mode)
{
    mouseMode_ = mode;

    switch (mode){
    case MouseMode::regular:
        setCursor(Qt::ArrowCursor);
        break;
    case MouseMode::selectPosition:
        setCursor(Qt::CrossCursor);
        break;
    case MouseMode::selectEntity:
        QCursor cur(QPixmap(":/resources/graphics/misc/targetMouseCursor.png"));
        setCursor(cur);
    }
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

void Game::updateGuiPositions()
{
    for (Gui* gui:guis_){
        QPointF newPos = mapToScene(gui->viewPos().toPoint());
        gui->graphicsItem_->setPos(newPos);
    }
}
