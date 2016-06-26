#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <set>
#include <unordered_set>

class Map;
class DynamicEntity;
class InventoryViewer;
class Entity;

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
    // enums
    enum class MouseMode { regular, selectPosition, selectEntity };

    // constructor
    Game(Map* map);

    void launch();

    void setMap(Map* map);
    Map* map();
    QPointF mapToMap(const QPoint &point);

    void setCamPos(QPointF to);
    QPointF camPos();
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
    void positionSelected(QPointF pos);
    void entitySelected(Entity* entity);

public slots:
    void askEnemiesToMove(); // TODO delete test
    void updatePosOverlays();
    void rainStep_();

private:
    // main private attributes
    Map* map_;
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
