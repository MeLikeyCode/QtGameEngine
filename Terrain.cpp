#include "Terrain.h"
#include <QRectF>
#include <QPixmap>
#include "Node.h"

Terrain::Terrain(int numXTiles, int numYTiles, int tileWidth, int tileHeight):
    numXTiles_(numXTiles),
    numYTiles_(numYTiles),
    tileWidth_(tileWidth),
    tileHeight_(tileHeight),
    grid_(numXTiles,numYTiles,tileWidth,tileHeight)
{
    // constructor body
    // calculate width_ and height_
    width_ = numXTiles * tileWidth;
    height_ = numYTiles * tileHeight;

    parentItem_ = new QGraphicsPixmapItem();
}

/// Fills the specified cell with the specified pixmap.
void Terrain::fill(const MyNode &cell, QPixmap pixmap){
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
    for (MyNode cell:grid_.nodes()){
        // fill it with the specified pixmap
        fill(cell,pixmap);
    }
}

/// Returns (a pointer to) the QGraphicsPixmapItem that is the parent of all the
/// Tiles.
///
/// This is an internal things, use with caution. I use it to add the parentItem
/// to the Map's scene when the Map's Terrain is set.
QGraphicsPixmapItem *Terrain::parentItem(){
    return parentItem_;
}
