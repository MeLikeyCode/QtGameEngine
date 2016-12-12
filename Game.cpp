#include "Game.h"

#include "Map.h"
#include <QMouseEvent>
#include "Spear.h" // TODO remove, test
#include "Entity.h"
#include "Slot.h"
#include "ProjectileMoveBehaviorHoming.h"
#include "Projectile.h"
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
#include <cassert>
#include "Utilities.h"
#include <QGraphicsItem>

#include "ECPathMover.h" // TODO: delete, test only

extern Spear* wSpear; // TODO: del, test

/// Creates an instance of the Game with some default options.
Game::Game(MapGrid *mapGrid, int xPosOfStartingMap, int yPosOfStartingMap):
    mapGrid_(mapGrid)
{
    for (Map* map:mapGrid_->maps()){
        map->setGame(this);
    }
    setMouseTracking(true);

    currentMap_ = mapGrid_->mapAt(xPosOfStartingMap,yPosOfStartingMap);
    setScene(currentMap_->scene());

    // disable QGraphicsView's scroll bars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    guiUpdateTimer_ = new QTimer(this);
    connect(guiUpdateTimer_,&QTimer::timeout,this,&Game::updateGuiPositions);
    guiUpdateTimer_->start(0);

    setMouseMode(MouseMode::regular);
}

/// Launches the Game.
void Game::launch(){
    showNormal(); // TODO: eventually should showFullscreen() (or parametrize to
                  // allow launching normal or full screen

     //showFullScreen();
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
        scene()->addItem(gui);
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

    // attack with spear
    wSpear->attack(mapToMap(event->pos()));


//    // add rock drawing and fill clicked cell
//    if (event->button() == Qt::MiddleButton){
//        // add rock
////        QPixmap pic(":resources/graphics/terrain/rock.png");
////        QGraphicsPixmapItem* picI = new QGraphicsPixmapItem(pic);
////        picI->setPos(event->pos().x()/64 * 64,event->pos().y()/64 * 64);
////        map_->scene()->addItem(picI);

//        map_->pathingMap().fill(mapToMap(event->pos()));
//        map_->drawPathingMap();
//    }

    QGraphicsView::mousePressEvent(event);
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
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

/// Adds the specified Gui to the game.
void Game::addGui(Gui *gui)
{
    guis_.insert(gui);
    scene()->addItem(gui);
    gui->setZValue(Map::Z_VALUES::GUI_Z_VALUE); // put Guis on top
    gui->setVisible(true); // when a gui is added to a game, it's always visible // TODO: why is this line needed?
}

/// Removes the specified Gui from the game.
void Game::removeGui(Gui *gui)
{
    scene()->removeItem(gui);
    guis_.erase(gui);
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

/// Adds a watched-watching pair.
/// If the specified watched-watching pair already exists, will simply
/// update the range.
/// @see watchedEntityEntersRange()
void Game::addWatchedEntity(Entity *watched, Entity *watching, double range)
{
    // make sure neither the watched nor the watching are nullptr
    assert(watched != nullptr && watching != nullptr);

    // init variables
    std::pair<Entity*, Entity*> watchedWatchingPair = std::make_pair(watched,watching);
    auto watchingSet = watchedToWatching_.find(watched); // set of watching entities for the entity

    // simply update range if this watched-watching pair already exists
    if (watchedWatchingPairExists(watched,watching)){
            watchedWatchingPairToRange_[watchedWatchingPair] = range;
            return;
    }

    // if watched has never been added, add it
    if (watchingSet == watchedToWatching_.end()){
        watchedToWatching_[watched] = std::set<Entity*>();
    }

    // add watching to watchingset
    watchedToWatching_[watched].insert(watching);

    // add range data
    watchedWatchingPairToRange_[watchedWatchingPair] = range;

    // add emitted data
    watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = false; // initially not emitted
}

/// Returns true if the specified watched-watching pair exists.
/// @see watchedEntityEntersRange()
bool Game::watchedWatchingPairExists(Entity *watched, Entity *watching)
{
    // make sure inputs are not nullptr
    assert(watched != nullptr && watching != nullptr);

    // if this pair is found return true
    auto watchingSet = watchedToWatching_.find(watched); // set of watching entities for the entity
    if (watchingSet != watchedToWatching_.end()){
        if (watchingSet->second.find(watching) != watchingSet->second.end()){
            return true;
        }
    }

    // otherwise, false
    return false;
}

/// Removes a watched-watching pair.
/// Throws if the specified watched-watching pair does not exist (call
/// watchedWatchingPairExists() to see if a pair exists before calling this function).
/// @see watchedEntityEntersRange();
void Game::removeWatchedEntity(Entity *watched, Entity *watching)
{
    // make sure neither inputs are nullptr
    assert(watched != nullptr && watching != nullptr);

    // make sure specified pair exists
    assert(watchedWatchingPairExists(watched,watching));

    // init variables
    std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(watched,watching);
    auto watchingSet = watchedToWatching_.find(watched)->second;
    auto watcher = watchingSet.find(watching);

    // earase
    watchingSet.erase(watcher);
    watchedWatchingPairToRange_.erase(watchedWatchingPair);
    watchedWatchingPairToEnterRangeEmitted_.erase(watchedWatchingPair);

    // if this was the last watching, erase the watched
    if (watchingSet.size() == 0)
        watchedToWatching_.erase(watched);
}

/// Completely removes the specified entity from the watched list.
/// This is the same as calling removeWatchedEntity(Entity*,Entity*) for
/// all the watched-watching pairs this watched is associated with.
/// Sorry for the tongue twister :).
/// /// @see watchedEntityEntersRange();
void Game::removeWatchedEntity(Entity *watched)
{
    // make sure input is not nullptr
    assert(watched != nullptr);

    // init variables
    std::set<Entity*> watchingSet = watchingEntities(watched);

    for (Entity* watchingEntity:watchingSet){
        removeWatchedEntity(watched,watchingEntity);
    }
}

/// Returns all the entities that are being watched.
/// @see watchedEntityEntersRange();
std::set<Entity *> Game::watchedEntities()
{
    std::set<Entity*> results;
    for (auto pair:watchedToWatching_){
        results.insert(pair.first);
    }
    return results;
}

/// Returns all the entities that are watching the specified entity.
/// @see watchedEntityEntersRange();
std::set<Entity *> Game::watchingEntities(Entity* of)
{
    // make sure input is not nullptr
    assert(of != nullptr);

    // init variables
    auto theSet = watchedToWatching_.find(of);
    if (theSet == watchedToWatching_.end())
        return std::set<Entity*>(); // return empty set
    else
        return theSet->second;
}

/// Returns the range that the watching entity is watching the watched entity.
/// @see watchedEntityEntersRange();
double Game::watchedWatchingRange(Entity *watched, Entity *watching)
{
    // make sure neither input is null
    assert(watched != nullptr && watching != nullptr);

    // make sure this pair actually exists
    assert(watchedWatchingPairExists(watched,watching));

    // init variables
    std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(watched,watching);

    return watchedWatchingPairToRange_[watchedWatchingPair];
}

/// Sets the range for the specified watced-watching pair.
/// @see watchedEntityEntersRange();
void Game::setWatchedWatchingRange(Entity *watched, Entity *watching, double range)
{
    // make sure not nullptr
    assert(watched != nullptr && watching != nullptr);

    // make sure range is positive
    assert(range >= 0);

    // make sure the pair exists
    assert(watchedWatchingPairExists(watched,watching));

    // init variables
    std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(watched,watching);
    watchedWatchingPairToRange_[watchedWatchingPair] = range;
}

void Game::updateGuiPositions()
{
    for (Gui* gui:guis_){
        QPointF newPos = mapToScene(gui->guiPos().toPoint());
        gui->setPos(newPos);
    }
}

/// Executed whenever an Entity moves.
/// Will see if the moved entity enters/leaves range of any watching entities, if so,
/// will emit respective signal.
/// @see watchedEntityEntersRange()
void Game::onEntityMoved(Entity *entity)
{
    // make sure entity is not null
    assert(entity != nullptr);

    // approach:
    // compare entity-watching and watching-entity distance and range to determine
    // if any watchedEntityEntersRange or watchedEntityLeavesRange signals should
    // be emitted

    for (Entity* watching:watchingEntities(entity)){
        // entity - watching variables
        std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(entity,watching);
        double dist = distance(entity->pointPos(),watching->pointPos());
        double range = watchedWatchingRange(entity,watching);
        bool alreadyInRange = watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair]; // already in range?

        if (dist < range && !alreadyInRange){
            emit watchedEntityEntersRange(entity,watching,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = true;
        }

        if (dist > range && alreadyInRange){
            emit watchedEntityLeavesRange(entity,watching,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = false;
        }

        if (watchedWatchingPairExists(watching,entity)){
            // watching - entity variables
            std::pair<Entity*,Entity*> watchedWatchingPair2 = std::make_pair(watching,entity);
            double distance2 = dist;
            double range2 = watchedWatchingRange(watching,entity);
            double alreadyInRange2 = watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair2];

            if (distance2 < range2 && !alreadyInRange2){
                emit watchedEntityEntersRange(watching,entity,range);
                watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair2] = true;
            }

            if (distance2 > range2 && alreadyInRange2){
                emit watchedEntityLeavesRange(watching,entity,range);
                watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair2] = false;
            }
        }
    }
}
