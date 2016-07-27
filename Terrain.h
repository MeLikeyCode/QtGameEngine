#ifndef TERRAIN_H
#define TERRAIN_H

// uses
#include <QPixmap>

// members:
#include "Grid.h"
#include <QGraphicsPixmapItem>

class Map;

/// Represents a bunch of tiles arranged in a grid. They get auto blended.
///
/// To create a Terrain:
/// Terrain* terrain = new Terrain(numXTiles,numYTiles,tileWidth,tileHeight);
///
/// To fill the entire terrain with a certain tile:
/// terrain->fill(QPixmap pixmap);
///
/// To fill a certain cell with a certain tile:
/// terrain->fill(const Node& cell, QPixmap pixmap);
class Terrain{
    friend class Map; // Map needs to access parentItem_
public:
    // constructor
    Terrain(int tileWidth, int tileHeight, int mapWidth, int mapHeight);

    void fill(const Node& cell, QPixmap pixmap);
    void fill(QPixmap pixmap);

private:
    // main attributes
    int numXTiles_;
    int numYTiles_;
    int tileWidth_;
    int tileHeight_;
    int width_;
    int height_;
    std::vector<QGraphicsPixmapItem*> tiles_;

    // helper
    Grid grid_;
    QGraphicsPixmapItem* parentItem_;

    // helper functions
    QGraphicsPixmapItem* pixmapAt_(int xPos, int yPos);
    void updateFade_(int x, int y);
    void fadeLeftOnly_(int x, int y);
    void fadeRightOnly_(int x, int y);
    void fadeTopOnly_(int x, int y);
    void fadeBottomOnly_(int x, int y);
    void fadeTopLeftOnly_(int x, int y);
    void fadeTopRightOnly_(int x, int y);
    void fadeBottomLeftOnly_(int x, int y);
    void fadeBottomRightOnly_(int x, int y);

    // TODO add other fades
};

#endif // TERRAIN_H
