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
/// A Map has a PathingMap which keeps track of which cells are free and
/// which are blocked. Each Entity can also contain its own PathingMap
/// therefore the Map is notified every time an Entity is added/moved
/// so that the Map's PathingMap can be updated accordingly.
///
/// A Map can be visualized by a Game. If the Map is currently being visualized
/// by a game the game() function will return that Game, otherwise it will return
/// nullptr.
class Map: public QObject // so we can use QPointer<Map>
{
    Q_OBJECT
    friend class Game; // game needs to be able to set game_ ptr of Map
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

signals:
    /// Emitted when the Map is set as the current Map for the game.
    void setAsCurrentMap();

    /// Emitted when the Map is unset as the current Map for the game.
    /// I.e. another Map becomes the current map of the game.
    void unsetAsCurrentMap();

    /// Emitted when the game camera moves around the Map.
    /// This is only emitted when this map is set as the current Map.
    void camMoved(QPointF newCamPos);

public slots:
    void onCamMoved_(QPointF newCamPos);
    void onMapChanged_(Map* oldMap, Map* newMap);

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
    void setGame_(Game* game);

};

#endif // MAP_H
