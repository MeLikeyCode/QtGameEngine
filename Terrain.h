#ifndef TERRAIN_H
#define TERRAIN_H

// uses
#include <QPixmap>

// members:
#include "Grid.h"
#include <QGraphicsPixmapItem>

/// Represents a bunch of tiles arranged in a grid.
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
public:
    // constructor
    Terrain(int numXTiles, int numYTiles, int tileWidth, int tileHeight);

    void fill(const MyNode& cell, QPixmap pixmap);
    void fill(QPixmap pixmap);

    // internal thing, needed by Map when terrain is changed. Terrain needs
    // to add this parent item to it's scene.
    QGraphicsPixmapItem* parentItem();

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
};

#endif // TERRAIN_H
