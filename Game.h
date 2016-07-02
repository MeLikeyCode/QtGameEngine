#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <set>
#include <unordered_set>

class Map;
class MapGrid;
class DynamicEntity;
class InventoryViewer;
class Entity;

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
    enum class Direction { up, down, left, right };

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

    void addInventoryViewer(InventoryViewer *viewer);

    std::set<int> keysPressed();
    QPoint getMousePos();
    MouseMode mouseMode();
    void setMouseMode(MouseMode mode);


    void setPlayer(DynamicEntity* player);
    DynamicEntity* player();

    std::vector<DynamicEntity*> enemies_; // TODO delete test

signals:
    /// Emitted Whenever a position is selected while Game is in selectPosition mode.
    void positionSelected(QPointF pos);

    /// Emitted whenever an Entity is selected while the Game is in select Entity mode.
    void entitySelected(Entity* entity);

    /// Emitted when the current Map of the game is changed.
    void mapChanged(Map* oldMap, Map* newMap);

public slots:
    void askEnemiesToMove(); // TODO delete test
    void updatePosOverlays();

private:
    // main private attributes
    MapGrid* mapGrid_;
    Map* currentMap_;
    std::set<int> keysPressed_;
    MouseMode mouseMode_;

    QTimer* updateTimer_;

    std::unordered_set<InventoryViewer*> inventoryViewers_;

    // TODO remove the following attributes, test attributes
    DynamicEntity* player_;
    std::vector<DynamicEntity*> testEntities_;
    std::vector<QGraphicsPixmapItem*> rains_;


};

#endif // GAME_H
