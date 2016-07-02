#include "Terrain.h"
#include <QRectF>
#include <QPixmap>
#include "Node.h"

Terrain::Terrain(int tileWidth, int tileHeight, int mapWidth, int mapHeight):
    tileWidth_(tileWidth),
    tileHeight_(tileHeight),
    width_(mapWidth),
    height_(mapHeight),
    parentItem_(new QGraphicsPixmapItem())
{
    numXTiles_ = mapWidth / tileWidth;
    numYTiles_ = mapHeight / tileHeight;

    grid_ = Grid(numXTiles_,numYTiles_,tileWidth_,tileHeight_);
}

/// Fills the specified cell with the specified pixmap.
void Terrain::fill(const Node &cell, QPixmap pixmap){
    // create the QGraphicsPixmapItem
    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(parentItem_);

    // keep track of it
    tiles_.push_back(pItem);

    // set its pixmap
    pItem->setPixmap(pixmap);

    // set its pos
    pItem->setPos(grid_.nodeToPoint(cell));
}

/// Fills the entire terrain with the specified pixmap.
void Terrain::fill(QPixmap pixmap){
    // for each cell (node) in the terrain
    for (Node cell:grid_.nodes()){
        // fill it with the specified pixmap
        fill(cell,pixmap);
    }
}
