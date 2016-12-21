#ifndef MAP_H
#define MAP_H

#include <vector>
#include "PathingMap.h"
#include <QPointer>
#include "Game.h"
#include <QObject>

class QPolygonF;
class QGraphicsItem;
class QGraphicsScene;
class TerrainLayer;
class Entity;
class Sprite;
class WeatherEffect;

/// Represents a map which can contain a bunch of interacting Entities.
///
/// The only thing that can be added to a map is an Entity (or one of its
/// sub classes).
///
/// A Map has a PathingMap which keeps track of which cells are free and
/// which are blocked. Each Entity can also contain its own PathingMap
/// therefore the Map is notified every time an Entity is added/moved
/// so that the Map's PathingMap can be updated accordingly.
class Map: public QObject // so we can use QPointer<Map>
{
    Q_OBJECT
public:
    enum Z_VALUES {
        GUI_Z_VALUE = 3,
        WEATHER_Z_VALUE = 2,
        BORDER_Z_VALUE = 1,
        ENTITY_Z_VALUE = 0,
        TERRAIN_Z_VALUE = -1
    };

    // constructor
    Map(PathingMap pathingMap);

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

    void addTerrainLayer(TerrainLayer* terrainLayer);

    QGraphicsScene* scene();

    Game* game();
    void setGame(Game* game);

    // for testing
    void drawPathingMap();

    double distance(Entity* e1, Entity* e2);

    /// getting all entities at a certain point/region
    Entity* closest(const QPointF& point);
    std::unordered_set<Entity*> entities(const QRectF& inRegion);
    std::unordered_set<Entity *> entities(const QPointF& atPoint);
    std::unordered_set<Entity*> entities(const QPolygonF& inRegion);
    std::unordered_set<Entity*> entities(Entity* collidingWith);
    std::unordered_set<Entity*> entities(const QRectF &inRegion,double zRangeMin, double zRangeMax);
    std::unordered_set<Entity*> entities(const QPointF &atPoint, double zRangeMin, double zRangeMax);
    std::unordered_set<Entity*> entities(const QPolygonF &inRegion, double zRangeMin, double zRangeMax);

    void playOnce(Sprite* sprite, std::string animation, int delaybwFramesMS, QPointF atPos);

    void setWeatherEffect(WeatherEffect* weatherEffect);
    WeatherEffect *weatherEffect();
private:
    int width_;
    int height_;
    int numCellsWide_;
    int numCellsLong_;
    int cellSize_;
    PathingMap pathingMap_;
    std::unordered_set<Entity*> entities_;
    std::vector<TerrainLayer*> terrainLayers_;
    QPointer<Game> game_;
    QPointer<WeatherEffect> weather_;

    // for testing
    std::vector<QGraphicsItem*> drawings_;

    QGraphicsScene* scene_;

    // helper functions
    void setFadingBorder_();

};

#endif // MAP_H
