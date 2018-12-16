#pragma once

#include "Vendor.h"

#include "PathingMap.h"
#include "Game.h"
#include "PositionalSound.h"
#include "TerrainLayer.h"

class QPolygonF;
class QGraphicsItem;
class QGraphicsScene;

namespace qge{

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
/// by a game the Map::game() function will return that Game, otherwise it will return
/// nullptr.
/// @author Abdullah Aghazadah
class Map: public QObject // so we can use QPointer<Map>
{
    Q_OBJECT
    friend class Game; // game needs to be able to set game_ ptr of Map (in Game::setCurrentMap())
public:
    // constructors
    Map();
    Map(int numCellsX, int numCellsY, double cellSize);
    Map(PathingMap* pathingMap);

    bool contains(const QPointF& pos);
    bool contains(Entity* entity);

    QPointF getMousePosition();

    PathingMap& pathingMap();
    void addPathingMap(PathingMap& pm, const QPointF& atPos);
    void removePathingMap(PathingMap& pm);
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
    void drawEntityBBoxes();

    /// getting all entities at a certain point/region/colliding w other entities
    std::unordered_set<Entity*> entities(const QRectF& inRect);
    std::unordered_set<Entity *> entities(const QPointF& atPoint);
    std::unordered_set<Entity*> entities(const QPolygonF& inRegion);
    std::unordered_set<Entity*> entities(Entity* collidingWith);
    std::unordered_set<Entity*> entities(const QRectF &inRegion,double zRangeMin, double zRangeMax);
    std::unordered_set<Entity*> entities(const QPointF &atPoint, double zRangeMin, double zRangeMax);
    std::unordered_set<Entity*> entities(const QPolygonF &inRegion, double zRangeMin, double zRangeMax);

    void playOnce(Sprite* sprite, std::string animation, int delaybwFramesMS, QPointF atPos);

    void addWeatherEffect(WeatherEffect &weatherEffect);
    void removeWeatherEffect(WeatherEffect &weatherEffect);

    void addPositionalSound(PositionalSound* sound);

    void addTerrainDecoration(const QPixmap& picture, const QPointF atPos);

signals:
    /// Emitted when the Map is set as the current Map for the game.
    void setAsCurrentMap(Map* sender);

    /// Emitted when the Map is unset as the current Map for the game.
    /// I.e. another Map becomes the current map of the game.
    void unsetAsCurrentMap(Map* sender);

    /// Emitted when the game camera moves around the Map.
    /// This is only emitted when this map is set as the current Map for the Game.
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
    PathingMap* ownPathingMap_; // Map's own pathing map, this can be arbitrarly filled by client
    std::unordered_map<PathingMap*,QPointF> additionalPathingMaps_; // additional pathing maps (and their location) that can be added/removed via addPathingMap()/removePathingMap()
    PathingMap* overallPathingMap_; // the "merge" of "ownPathingMap_" and all "additionalPathingMaps_"
    std::unordered_set<Entity*> entities_;
    std::vector<std::unique_ptr<TerrainLayer>> terrainLayers_;
    QPointer<Game> game_; // the game that is currently visualizing the map, null if not currently being visualizing
    std::set<WeatherEffect*> weatherEffects_;

    QGraphicsRectItem* weatherLayer_;
    QGraphicsRectItem* entityLayer_;
    QGraphicsRectItem* terrainLayer_;

    // for testing
    std::vector<QGraphicsItem*> drawings_;

    QGraphicsScene* scene_;

    // helper functions
    void setFadingBorder_();
    void setGame_(Game* game);
};

}
