#pragma once

#include "Vendor.h"

#include "DiplomacyManager.h"

namespace qge{

class Map;
class MapGrid;
class Entity;
class InventoryViewer;
class Entity;
class Gui;

/// Represents the game as a whole.
/// This is basically the window the will visualize a Map.
/// This class is a singleton, thus you can only construct one instance.
/// If you try to construct a second one, an exception will be thrown.
/// The Game::game pointer is global, you can access it from anywhere.
/// @author Abdullah Aghazadah
/// @date 6-3-15
class Game: public QGraphicsView{
    Q_OBJECT //todo delete test
public:
    // enums
    enum class MouseMode { regular, selectPosition, selectEntity };

    // constructor
    static Game* game;
    Game(MapGrid* mapGrid, int xPosOfStartingMap, int yPosOfStartingMap);

    void launch();

    MapGrid* mapGrid();
    void setCurrentMap(Map* currentMap);
    Map* currentMap();
    QPointF mapToMap(const QPoint &point);
    QPointF mapFromMap(const QPointF &point);

    // camera controls
    void setCenterCamPos(QPointF position);
    QPointF centerCamPos();
    QRectF cam();
    void moveCam(QVector2D byVector);
    void moveCamUp(double byAmount);
    void moveCamDown(double byAmount);
    void moveCamLeft(double byAmount);
    void moveCamRight(double byAmount);

    // mouse event handlers
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void addGui(Gui *gui);
    void removeGui(Gui *gui);
    bool containsGui(Gui* gui);

    std::set<int> keysPressed();
    QPoint getMousePos();
    MouseMode mouseMode();
    void setMouseMode(MouseMode mode);

    std::vector<Entity*> enemies_; // TODO delete test

    void addWatchedEntity(Entity* watched, Entity* watching, double range);
    bool watchedWatchingPairExists(Entity* watched, Entity* watching);
    void removeWatchedEntity(Entity* watched, Entity* watching);
    void removeWatchedEntity(Entity* watched);
    void removeWatchingEntity(Entity* watching);
    std::set<Entity*> watchedEntities();
    std::set<Entity*> watchedEntities(Entity* of);
    std::set<Entity*> watchingEntities();
    std::set<Entity*> watchingEntities(Entity *of);
    double watchedWatchingRange(Entity* watched, Entity* watching);
    void setWatchedWatchingRange(Entity* watched, Entity* watching, double range);

    DiplomacyManager& diplomacyManager();

signals:
    /// Emitted whenever a position is selected while Game is in selectPosition MouseMode.
    /// @param pos The position that was selected, in Map coordinates (of the current Map).
    void positionSelected(QPointF pos);

    /// Emitted whenever an Entity is selected while the Game is in selectEntity MouseMode.
    void entitySelected(Entity* entity);

    /// Emitted whenever the a mouse button is pressed down while Game is in regular MouseMode.
    /// @param pos The position selected, in screen coordinates.
    /// @param button The mouse button that was pressed.
    void mousePressed(QPoint pos, Qt::MouseButton button);

    /// Emitted whenever the mouse is moved while the Game is in regular MouseMode.
    /// @param The position (in screen coordinates) that the mouse moved to.
    void mouseMoved(QPoint pos);

    /// Emitted whenever a mouse button is released while the Game is in regular MouseMode.
    /// @param The position (in screen coordinates) at which the mouse was released at.
    /// @param button The mouse button that was released.
    void mouseReleased(QPoint pos, Qt::MouseButton button);

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

    /// Emitted Game's camera moves.
    /// @param newPos the new position of the camera (its center).
    void camMoved(QPointF newPos);

public slots:
    void updateGuiPositions();
    void onEntityMoved(Entity* entity);
    void onWatchedEntityDies_(QObject *watchedEntity);
    void onWatchingEntityDies_(QObject *watchingEntity);

private:
    // main private attributes
    MapGrid* mapGrid_;
    Map* currentMap_;
    std::set<int> keysPressed_;
    MouseMode mouseMode_;

    QTimer* guiUpdateTimer_;
    std::unordered_set<Gui*> guis_;

    QGraphicsRectItem* guiLayer_;

    DiplomacyManager diplomacyManager_;

    // watched/watching entities
    std::map<Entity*, std::set<Entity*>> watchedToWatching_; // each Entity that is watched to the Entities that are watching it
    std::map<Entity*, std::set<Entity*>> watchingToWatched_; // each Entity that is watching to the Entities that its watching
    std::map<std::pair<Entity*,Entity*>,double> watchedWatchingPairToRange_; // for each watched/watching pair, what is the range?
    std::map<std::pair<Entity*,Entity*>,bool> watchedWatchingPairToEnterRangeEmitted_; // has the enters range event been emitted yet for a pair of watched watching entities?
};

}
