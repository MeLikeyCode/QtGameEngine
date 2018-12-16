#pragma once

#include "Vendor.h"

#include "Node.h"
#include "Grid.h"

namespace qge{

class Map;

/// Represents a bunch of auto blended QPixmaps arranged in a grid that can go inside a Map. You
/// can set the QPixmap to use for the Terrain by calling setPixmap(QPixmap). You can fill/unfill
/// certain cells by calling fill(const Node&)/unfill(const Node&). You set the size of cells by
/// calling setTileWidth/setTileHeight(). You can set the position of the TerrainLayer by using
/// setPos(QPointF).
class TerrainLayer{
    friend class Map; // Map needs to access parentItem_
public:
    // static functions
    static void setAlphaChannel(QPixmap& pixmap, int const alpha);

    // constructor
    TerrainLayer(int numXTiles, int numYTiles);
    TerrainLayer(int numXTiles, int numYTiles, QPixmap pixmap);


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

}
