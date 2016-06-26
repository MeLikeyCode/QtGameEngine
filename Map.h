#ifndef MAP_H
#define MAP_H

#include <vector>
#include "PathingMap.h"

class QPolygonF;
class QGraphicsItem;
class QGraphicsScene;
class Terrain;
class Game;
class Entity;
class Sprite;

/// Represents a map which can contain a bunch of interacting Entities.
///
/// The only thing that can be added to a map is an Entity (or one of its
/// derivitives).
///
/// A Map has a PathingMap which keeps track of which cells are free and
/// which are blocked. Each Entity can also contain its own PathingMap,
/// therefore the Map is notified every time an Entity is added/moved
/// so that the Map's PathingMap can be updated accordingly.
class Map{
public:
    // constructor
    Map(int numCellsWide=20, int numCellsLong=20, int cellSize=64);

    bool contains(const QPointF& pos);
    bool contains(Entity* entity);

    QPointF getMousePosition();

    PathingMap& pathingMap();
    void updatePathingMap();

    int width() const;
    int height() const;
    QSizeF size() const;

    int numCellsWide() const;
    int numCellsLong() const;
    int cellSize() const;
    QPointF cellToPoint(const Node &cell);
    Node pointToCell(const QPointF& point);

    std::unordered_set<Entity*> entities();
    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    void setTerrain(Terrain* to);

    QGraphicsScene* scene();

    Game* game();
    void setGame(Game* game);

    // for testing
    void drawPathingMap();

    double distance(Entity* e1, Entity* e2);

    Entity* closest(const QPointF& point);
    std::unordered_set<Entity*> entities(const QRectF& inRegion);
    std::unordered_set<Entity *> entities(const QPointF& atPoint);
    std::unordered_set<Entity*> entities(const QPolygonF& inRegion);

    void playOnce(Sprite* sprite, std::string animation, int delaybwFramesMS, QPointF atPos);
private:
    int width_;
    int height_;
    int numCellsWide_;
    int numCellsLong_;
    int cellSize_;
    PathingMap pathingMap_;
    std::unordered_set<Entity*> entities_;
    Terrain* terrain_;
    Game* game_;

    // for testing
    std::vector<QGraphicsItem*> drawings_;

    QGraphicsScene* scene_;

};

#endif // MAP_H
