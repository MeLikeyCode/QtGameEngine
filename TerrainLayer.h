#ifndef TERRAIN_H
#define TERRAIN_H

#include <QPixmap>
#include "Node.h"
#include "Grid.h"
#include <QPointF>
#include <QGraphicsPixmapItem>

class Map;

/// Represents a bunch of QPixmaps arranged in a grid. They get auto blended.
/// You can set the QPixmap to use for the Terrain by calling setPixmap(QPixmap).
/// Once you have set the QPixmap, you can start filling certain cells with
/// that pixmap by calling fill(const Node). You can unfill cells by calling
/// unfill(const Node). You can always change the pixmap by calling setPixmap(QPixmap)
/// again. You can always change the size of cells by calling setTileWidth/setTileHeight().
/// You can set the position of the TerrainLayer by using setPos(QPointF).
class TerrainLayer{
    friend class Map; // Map needs to access parentItem_
public:
    // constructor
    TerrainLayer(int tileWidth, int tileHeight, int numXTiles, int numYTiles, QPixmap pixmap);


    void setPos(const QPointF& pos);
    QPointF pos();
    void setPixmap(QPixmap pixmap);
    void setTileWidth(double width);
    void setTileHeight(double height);

    void fill(const Node& cell);
    void unfill(const Node& cell);
    bool filled(const Node& cell);
    void fill();

private:
    // main attributes
    int numXTiles_;
    int numYTiles_;
    int tileWidth_;
    int tileHeight_;
    QPixmap pixmap_;
    QPointF pos_;
    std::vector<QGraphicsPixmapItem*> tiles_;

    // constants (TODO: make variable)
    double linearFadePercent_;

    // helper
    Grid grid_;
    QGraphicsPixmapItem* parentItem_;

    // helper functions
    QGraphicsPixmapItem* pixmapAt_(int xPos, int yPos);
    Node positionOf_(QGraphicsPixmapItem* pixmapItem);
    void setCorrectFade_(int x, int y);
    void fadeLeftOnly_(int x, int y);
    void fadeRightOnly_(int x, int y);
    void fadeTopOnly_(int x, int y);
    void fadeBottomOnly_(int x, int y);
    void fadeTopLeftOnly_(int x, int y);
    void fadeTopRightOnly_(int x, int y);
    void fadeBottomLeftOnly_(int x, int y);
    void fadeBottomRightOnly_(int x, int y);

    void draw_();
};

#endif // TERRAIN_H
