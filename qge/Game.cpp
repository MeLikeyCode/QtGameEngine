#include "Game.h"

#include "Map.h"
#include "Spear.h" // TODO remove, test
#include "Entity.h"
#include "Slot.h"
#include "Projectile.h"
#include "WeaponSlot.h"
#include "Axe.h"
#include "Bow.h"
#include "InventoryViewer.h"
#include "WeatherEffect.h"
#include "RainWeather.h"
#include "MapGrid.h"
#include "Gui.h"
#include "TopDownSprite.h"
#include "QtUtilities.h"
#include "STLWrappers.h"

#include "ECPathMover.h" // TODO: delete, test only
#include "ECBodyThruster.h" // TODO: delete, test only
#include "Sprite.h" // TODO: delete, test only
#include "RangedWeaponSlot.h" // TODO: delete, test only
#include "MeleeWeaponSlot.h"

using namespace qge;

Game* Game::game = nullptr;

/// Creates an instance of the Game with some default options.
Game::Game(MapGrid *mapGrid, int xPosOfStartingMap, int yPosOfStartingMap):
    mapGrid_(mapGrid),
    guiLayer_(new QGraphicsRectItem())
{
    if (game != nullptr)
        std::logic_error("Game is a singleton. You can only construct one instance.");
    Game::game = this;

    guiLayer_->setZValue(INFINITY);

    // register types that needed to be used in cross thread signal-slot stuff
    qRegisterMetaType<PathingMap>();
    qRegisterMetaType<std::vector<QPointF>>();

    for (Map* map:mapGrid_->maps()){
        map->setGame_(this);
    }
    setMouseTracking(true);

    currentMap_ = mapGrid_->mapAt(xPosOfStartingMap,yPosOfStartingMap);
    setScene(currentMap_->scene());
    scene()->addItem(guiLayer_);

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
    setFixedSize(1280,720);
//    showFullScreen();
    setSceneRect(0,0,width(),height());
}

MapGrid *Game::mapGrid()
{
    return mapGrid_;
}

/// Sets the current Map that the Game is visualizing.
void Game::setCurrentMap(Map *map){
    assert(map != nullptr);

    scene()->removeItem(guiLayer_); // remove guis from previous map

    Map* oldMap = currentMap_;
    currentMap_ = map;

    setScene(currentMap_->scene());
    setSceneRect(0,0,width(),height());
    map->setGame_(this);

    scene()->addItem(guiLayer_); // add guis to new map

    // emit map changed signal
    if (oldMap != currentMap_)
        emit mapChanged(oldMap,map);
}

/// Returns the Game's current Map.
/// The current Map is the one currently being visualized.
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

    emit camMoved(position);
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

    emit camMoved(centerCamPos());
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
    // do normal QGraphicsView event handling (it will pass it to the QGraphicsItems at location).
    QGraphicsView::mousePressEvent(event);

    // if event was handled (by one of the QGraphicsItems), we're done
    if (event->isAccepted())
        return;

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

    // if execution got to here, mouseMode_ must be regular, just emit mousePressed event
    emit mousePressed(event->pos(),event->button());
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    // do normal QGraphicsView event handling (it will pass the even to QGraphicsItems)
    QGraphicsView::mouseMoveEvent(event);

    // if one of the QGraphicsItems handled the event, we're done
    if (event->isAccepted())
        return;

    // otherwise, simply emit mouseMove signal
    emit mouseMoved(event->pos());
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    // do normal QGraphicsView event handling (passing to QGraphicsItems)
    QGraphicsView::mouseReleaseEvent(event);

    // if one of the QGraphicsItems handled the event, we're done
    if (event->isAccepted())
        return;

    // otherwise, simply emit mouseReleased event
    emit mouseReleased(event->pos(), event->button());
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        close();
    }

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

/// Adds the specified Gui to the game.
void Game::addGui(Gui *gui)
{
    // prevent double adds
    if (STLWrappers::contains(guis_,gui))
        return;

    gui->game_ = this;
    gui->getGraphicsItem()->setParentItem(guiLayer_);
    guis_.insert(gui);
    gui->getGraphicsItem()->setVisible(true); // when a gui is added to a game, it's always visible // TODO remove: why is this here?
}

/// Removes the specified Gui from the game.
/// Does nothing if the gui is already removed.
void Game::removeGui(Gui *gui)
{
    // prevent removing something that isn't even here
    if (!STLWrappers::contains(guis_,gui))
        return;

    gui->setParentGui(nullptr);
    scene()->removeItem(gui->getGraphicsItem());
    guis_.erase(gui);
}

/// Returns true if the Game contains the specified Gui.
bool Game::containsGui(Gui *gui)
{
    return STLWrappers::contains(guis_,gui);
}

/// Converts the specified point from Game coordinates to Map coordinates (
/// of the current Map).
QPointF Game::mapToMap(const QPoint& point)
{
    return mapToScene(point);
}

/// Converts the specified point from Map coordinates to Game coordinates.
QPointF Game::mapFromMap(const QPointF &point)
{
    return mapFromScene(point);
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
    auto watchingSet = watchedToWatching_.find(watched); // 1 watched entity to * watching entities
    auto watchedSet = watchingToWatched_.find(watching); // 1 watching entit to * watched entities

    // simply update range if this watched-watching pair already exists
    if (watchedWatchingPairExists(watched,watching)){
            watchedWatchingPairToRange_[watchedWatchingPair] = range;
            return;
    }

    // if this is the first time the "watched" entity is being watched, add a set for it
    if (watchingSet == watchedToWatching_.end()){
        watchedToWatching_[watched] = std::set<Entity*>();
    }

    // if this is the first time the watching entity is watching anything, add a set for it
    if (watchedSet == watchingToWatched_.end()){
        watchingToWatched_[watching] = std::set<Entity*>();
    }

    watchedToWatching_[watched].insert(watching);
    watchingToWatched_[watching].insert(watched);

    // add range data
    watchedWatchingPairToRange_[watchedWatchingPair] = range;

    // add emitted data
    watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = false; // initially not emitted

    // listen to when either the watched or watching entities die
    // (so we can update book keeping)
    connect(watched,&QObject::destroyed,this,&Game::onWatchedEntityDies_);
    connect(watching,&QObject::destroyed,this,&Game::onWatchingEntityDies_);
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
    auto iter = watchingToWatched_.find(watching);
    if (iter == watchingToWatched_.end())
        bool b = true;
    auto watchedSet = watchingToWatched_.find(watching)->second;
    auto watcher = watchingSet.find(watching);
    auto watchee = watchedSet.find(watched);

    // earase
    watchingSet.erase(watcher);
    watchedSet.erase(watchee);
    watchedWatchingPairToRange_.erase(watchedWatchingPair);
    watchedWatchingPairToEnterRangeEmitted_.erase(watchedWatchingPair);
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

/// Removes all the watched-watching pairs where the specified entity is the
/// watching entity.
/// In other words, effectively makes the specified entity stop watching all its
/// watched entities.
void Game::removeWatchingEntity(Entity *watching)
{
    std::set<Entity*> ents = watchedEntities(watching);
    for (Entity* entity:ents)
        removeWatchedEntity(entity,watching);
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

/// Returns all entities that are being watched by the specified entity.
std::set<Entity *> Game::watchedEntities(Entity *of)
{
    assert(of != nullptr);

    auto theSet = watchingToWatched_.find(of);
    if (theSet == watchingToWatched_.end())
        return std::set<Entity*>(); // return empty set
    else
        return theSet->second;
}

/// Returns all entities that are watching some other entities.
/// @see watchedEntityEntersRange()
std::set<Entity *> Game::watchingEntities()
{
    std::set<Entity*> results;
    for (auto pair:watchingToWatched_){
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

DiplomacyManager &Game::diplomacyManager()
{
    return diplomacyManager_;
}

void Game::updateGuiPositions()
{
    guiLayer_->setPos(mapToScene(QPoint(0,0)));
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

    // if the entity that just moved is being watched,
    // get all entites that are watching him,
    // and see if he just entered or left within range of em
    for (Entity* watching:watchingEntities(entity)){
        // entity - watching variables
        std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(entity,watching);
        double dist = QtUtils::distance(entity->pos(),watching->pos());
        double range = watchedWatchingRange(entity,watching);
        bool alreadyInRange = watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair]; // already in range?

        if (dist < range && !alreadyInRange){
            emit watchedEntityEntersRange(entity,watching,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = true;
            return;
        }

        if (dist > range && alreadyInRange){
            emit watchedEntityLeavesRange(entity,watching,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = false;
            return;
        }
    }

    // if the entity that just moved is watching some other entities,
    // get these entities, and see if he entered/left range of any of em
    for (Entity* watched:watchedEntities(entity)){
        // entity - watching variables
        std::pair<Entity*,Entity*> watchedWatchingPair = std::make_pair(watched,entity);
        double dist = QtUtils::distance(entity->pos(),watched->pos());
        double range = watchedWatchingRange(watched,entity);
        bool alreadyInRange = watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair];

        if (dist < range && !alreadyInRange){
            emit watchedEntityEntersRange(watched,entity,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = true;
            return;
        }

        if (dist > range && alreadyInRange){
            emit watchedEntityLeavesRange(watched,entity,range);
            watchedWatchingPairToEnterRangeEmitted_[watchedWatchingPair] = false;
            return;
        }
    }

}

/// Executed when a watched entity dies.
/// Will update book keeping.
void Game::onWatchedEntityDies_(QObject *watchedEntity)
{
    removeWatchedEntity(static_cast<Entity*>(watchedEntity));
}

/// Executed when a watching entity dies.
/// Will update book keeping.
void Game::onWatchingEntityDies_(QObject *watchingEntity)
{
    removeWatchingEntity(static_cast<Entity*>(watchingEntity));
}
