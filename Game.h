#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <set>
#include <unordered_set>

class Map;
class MapGrid;
class Entity;
class InventoryViewer;
class Entity;
class Gui;

/// Represents an instance of the game.
/// @author Abdullah Aghazadah
/// @date 6-3-15
///
/// A Game represents an instance of the game as a whole.
/// It controls things such as GUIs, screen size, changing maps, etc...
class Game: public QGraphicsView{
    Q_OBJECT //todo delete test
public:
    // enums
    enum class MouseMode { regular, selectPosition, selectEntity };

    // constructor
    Game(MapGrid* mapGrid, int xPosOfStartingMap, int yPosOfStartingMap);

    void launch();

    MapGrid* mapGrid();
    void setCurrentMap(Map* currentMap);
    Map* currentMap();
    QPointF mapToMap(const QPoint &point);

    void setCenterCamPos(QPointF position);
    QPointF centerCamPos();
    QRectF cam();
    void moveCam(QVector2D byVector);
    void moveCamUp(double byAmount);
    void moveCamDown(double byAmount);
    void moveCamLeft(double byAmount);
    void moveCamRight(double byAmount);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void addGui(Gui *gui);
    void removeGui(Gui *gui);

    std::set<int> keysPressed();
    QPoint getMousePos();
    MouseMode mouseMode();
    void setMouseMode(MouseMode mode);


    void setPlayer(Entity* player);
    Entity* player();

    std::vector<Entity*> enemies_; // TODO delete test

    void addWatchedEntity(Entity* watched, Entity* watching, double range);
    bool watchedWatchingPairExists(Entity* watched, Entity* watching);
    void removeWatchedEntity(Entity* watched, Entity* watching);
    void removeWatchedEntity(Entity* watched);
    std::set<Entity*> watchedEntities();
    std::set<Entity*> watchingEntities(Entity *of);
    double watchedWatchingRange(Entity* watched, Entity* watching);
    void setWatchedWatchingRange(Entity* watched, Entity* watching, double range);

signals:
    /// Emitted Whenever a position is selected while Game is in selectPosition mode.
    void positionSelected(QPointF pos);

    /// Emitted whenever an Entity is selected while the Game is in select Entity mode.
    void entitySelected(Entity* entity);

    /// Emitted when the current Map of the game is changed.
    void mapChanged(Map* oldMap, Map* newMap);

    /// Emitted when a watched Entity enters within range of one of its watching entities.
    /// Game allows you to keep track of some Entities that are being watched by some other Entities.
    /// Whenever a "watched" entity moves, game will determine if its within range of any of the
    /// entities that are watching it (i.e. the "watching" entities), if so, a signal will be
    /// emitted.
    ///
    /// For example, suppose you have entityA and entityB. You want entityA to be notified
    /// whenever entityB enters within 100 pixels of it. You can do this by calling
    /// addWatchedEntity(entityB, entityA, 100);
    /// @param range the range of the watching entity that the watched entity just entered.
    void watchedEntityEntersRange(Entity* watched, Entity* watching, double range);

    /// Emitted when a watched Entity leaves range of one of its watching entities.
    /// @see watchedEntityEntersRange()
    /// @param range the range of the wathing entity that the watched entity just left.
    void watchedEntityLeavesRange(Entity* watched, Entity* watching, double range);

public slots:
    void askEnemiesToMove(); // TODO delete test
    void updateGuiPositions();
    void onEntityMoved(Entity* entity);

private:
    // main private attributes
    MapGrid* mapGrid_;
    Map* currentMap_;
    std::set<int> keysPressed_;
    MouseMode mouseMode_;

    QTimer* guiUpdateTimer_;
    std::unordered_set<Gui*> guis_;

    // watched/watching entities
    std::map<Entity*, std::set<Entity*>> watchedToWatching_; // each Entity that is watched to the Entities that are watching it
    std::map<std::pair<Entity*,Entity*>,double> watchedWatchingPairToRange_; // for each watched/watching pair, what is the range?
    std::map<std::pair<Entity*,Entity*>,bool> watchedWatchingPairToEnterRangeEmitted_; // has the enters range event been emitted yet for a pair of watched watching entities?


    // TODO remove the following attributes, test attributes
    Entity* player_;
    std::vector<Entity*> testEntities_;
    std::vector<QGraphicsPixmapItem*> rains_;


};

#endif // GAME_H
