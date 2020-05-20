#pragma once

#include "Vendor.h"

#include "Node.h"
#include "Grid.h"

namespace qge{

class Map;

/// Represents a grid with a bunch of cells; each of its cells can contain a QPixmap or it can be empty.
///
/// This grid can be placed in a Map by using the Map's `addTerrainLayer()` method. QPixmaps in a grid will be
/// nicely blended. I can see this class being very useful for a tile map kind of effect.
///
/// Example
/// -------
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// TerrainLayer* tl = new TerrainLayer(10,10,myPixmap); // Create a 10x10 grid, each filled cell will contain myPixmap
/// // Note: By default all cells are filled, use unfill() to unfill specified cells
/// tl->setPos(QPointF(100,100)); // place the TerrainLayer's top left corner at (100,100) of the Map
/// Map* map;
/// map->addTerrainLayer(tl); // add the TerrainLayer to the Map (will go on top of any other TerrainLayers of the Map)
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// @note You cannot place 2 *different* QPixmaps in the same TerrainLayer, you choose one QPixmap, and then you mark a cell
/// as either filled or not. If a cell is marked as filled, the QPixmap will appear there, otherwise it won't. You can add
/// multiple TerrainLayers in a Map to compensate for this fact.
///
/// Functions exist for setting the pixmap to be used, for filling/unfilling cells, for setting the size of cells,
/// and for setting the position of the top left corner of the TerrainLayer in the Map. If you want to disable the nice
/// blending/fading, call setEnableFade(false).
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

    void setEnableFade(bool flag);

private:
    // main attributes
    int numXTiles_;
    int numYTiles_;
    int tileWidth_;
    int tileHeight_;
    QPixmap pixmap_;
    QPointF pos_;
    std::vector<QGraphicsPixmapItem*> tiles_;
    bool enableFade_;

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
