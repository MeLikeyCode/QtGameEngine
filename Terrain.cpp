#include "Terrain.h"
#include <QRectF>
#include <QPixmap>
#include "Node.h"
#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>

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

    // initialize all pixmaps to null
    for (int i = 0, n = numXTiles_* numYTiles_; i < n; i++){
        tiles_.push_back(nullptr);
    }
}

/// Fills the specified cell with the specified pixmap.
void Terrain::fill(const Node &cell, QPixmap pixmap){
    // TODO: if there is already a tile, remove it first
    QGraphicsPixmapItem* oldTile = pixmapAt_(cell.x(),cell.y());
    if (oldTile != nullptr){
        QGraphicsScene* oldTileScene = oldTile->scene();
        if (oldTileScene){
            oldTileScene->removeItem(oldTile);
        }
        tiles_.erase(std::find(tiles_.begin(),tiles_.end(),oldTile));
    }

    // create the QGraphicsPixmapItem
    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(parentItem_);
    tiles_.push_back(pItem);
    pItem->setPixmap(pixmap);
    pItem->setPos(grid_.nodeToPoint(cell));

    // mark cell filled
    int posInArray = cell.y() * numXTiles_ + cell.x();
    tiles_[posInArray] = pItem;

    // update
    updateFade_(cell.x(),cell.y());

    // update top
    QGraphicsPixmapItem* top = pixmapAt_(cell.x(),cell.y() - 1);
    if (top){
        updateFade_(cell.x(),cell.y() - 1);
    }

    // update bottom
    QGraphicsPixmapItem* bottom = pixmapAt_(cell.x(),cell.y() + 1);
    if (bottom){
        updateFade_(cell.x(),cell.y() + 1);
    }

    // update left
    QGraphicsPixmapItem* left = pixmapAt_(cell.x()-1,cell.y());
    if (left){
        updateFade_(cell.x()-1,cell.y());
    }

    // update right
    QGraphicsPixmapItem* right = pixmapAt_(cell.x()+1,cell.y());
    if (right){
        updateFade_(cell.x()+1,cell.y());
    }

    // update topleft
    QGraphicsPixmapItem* topLeft = pixmapAt_(cell.x()-1,cell.y() - 1);
    if (topLeft){
        updateFade_(cell.x()-1,cell.y() -1);
    }

    // update topright
    QGraphicsPixmapItem* topRight = pixmapAt_(cell.x()+1,cell.y() - 1);
    if (topRight){
        updateFade_(cell.x()+1,cell.y() -1);
    }

    // update bottomleft
    QGraphicsPixmapItem* bottomLeft = pixmapAt_(cell.x()-1,cell.y() + 1);
    if (bottomLeft){
        updateFade_(cell.x()-1,cell.y() +1);
    }

    // update bottomright
    QGraphicsPixmapItem* bottomRight = pixmapAt_(cell.x()+1,cell.y() + 1);
    if (bottomRight){
        updateFade_(cell.x()+1,cell.y() +1);
    }
}

/// Fills the entire terrain with the specified pixmap.
void Terrain::fill(QPixmap pixmap){
    // for each cell (node) in the terrain
    for (Node cell:grid_.nodes()){
        // fill it with the specified pixmap
        fill(cell,pixmap);
    }
}

/// Returns the pixmap at the specified location. If there is none, returns nullptr.
QGraphicsPixmapItem *Terrain::pixmapAt_(int xPos, int yPos)
{
    if (xPos < 0 || xPos >= numXTiles_ || yPos < 0 || yPos >= numYTiles_){
        return nullptr;
    }

    int posInArray = yPos * numXTiles_ + xPos;
    return tiles_[posInArray];
}

/// Executed whenver the specified cell needs to recalculate itsfade.
void Terrain::updateFade_(int x, int y)
{
    // find out if its top, left, middle, etc...
    bool isTopLeft = false;
    bool isTopRight = false;
    bool isBottomLeft = false;
    bool isBottomRight = false;

    bool isTop = false;
    bool isBottom = false;
    bool isLeft = false;
    bool isRight = false;

    bool isMiddle = false;

    QGraphicsPixmapItem* topLeft = pixmapAt_(x-1,y-1);
    QGraphicsPixmapItem* topRight = pixmapAt_(x+1,y-1);
    QGraphicsPixmapItem* bottomLeft = pixmapAt_(x-1,y+1);
    QGraphicsPixmapItem* bottomRight = pixmapAt_(x+1,y+1);

    QGraphicsPixmapItem* top = pixmapAt_(x,y-1);
    QGraphicsPixmapItem* bottom = pixmapAt_(x,y+1);
    QGraphicsPixmapItem* left = pixmapAt_(x-1,y);
    QGraphicsPixmapItem* right = pixmapAt_(x+1,y);

    // see where this cell is
    isTopLeft = !top && !left && right && bottom;
    isTopRight = !top && !right && bottom && left;
    isBottomLeft = !left && !bottom && top && right;
    isBottomRight = !right && !bottom && top && left;
    isTop = !top && right && left && bottom;
    isBottom = !bottom && right && left && top;
    isLeft = !left && right && top && bottom;
    isRight = !right && left && top && bottom;
    isMiddle = top && bottom && left && right;

    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);
    if (pItem){
        if (isMiddle){
            pItem->setGraphicsEffect(nullptr);
        }
        if (isLeft){
            fadeLeftOnly_(x,y);
        }
        if (isRight){
            fadeRightOnly_(x,y);
        }
        if (isTop){
            fadeTopOnly_(x,y);
        }
        if (isBottom){
            fadeBottomOnly_(x,y);
        }
        if (isTopLeft){
            fadeTopLeftOnly_(x,y);
        }
        if (isTopRight){
            fadeTopRightOnly_(x,y);
        }
        if (isBottomLeft){
            fadeBottomLeftOnly_(x,y);
        }
        if (isBottomRight){
            fadeBottomRightOnly_(x,y);
        }
    }
}

/// Fades only the left side of the specified cell.
void Terrain::fadeLeftOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(50,0);
    lg.setFinalStop(0,0);
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeRightOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(pItem->boundingRect().width() - 50,0);
    lg.setFinalStop(pItem->boundingRect().width(),0);
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeTopOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(0,50);
    lg.setFinalStop(0,0);
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeBottomOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(0,pItem->boundingRect().height()-50);
    lg.setFinalStop(0,pItem->boundingRect().height());
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeTopLeftOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();

    QRadialGradient radialGradient;
    radialGradient.setCenter(pItem->boundingRect().width(), pItem->boundingRect().height());
    radialGradient.setFocalPoint(radialGradient.center());
    radialGradient.setRadius(pItem->boundingRect().width());
    radialGradient.setColorAt(0,Qt::black);
    radialGradient.setColorAt(0.8,Qt::black);
    radialGradient.setColorAt(1,Qt::transparent);

    opacity->setOpacityMask(radialGradient);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeTopRightOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();

    QRadialGradient radialGradient;
    radialGradient.setCenter(0, pItem->boundingRect().height());
    radialGradient.setFocalPoint(radialGradient.center());
    radialGradient.setRadius(pItem->boundingRect().width());
    radialGradient.setColorAt(0,Qt::black);
    radialGradient.setColorAt(0.8,Qt::black);
    radialGradient.setColorAt(1,Qt::transparent);

    opacity->setOpacityMask(radialGradient);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeBottomLeftOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();

    QRadialGradient radialGradient;
    radialGradient.setCenter(pItem->boundingRect().width(), 0);
    radialGradient.setFocalPoint(radialGradient.center());
    radialGradient.setRadius(pItem->boundingRect().width());
    radialGradient.setColorAt(0,Qt::black);
    radialGradient.setColorAt(0.8,Qt::black);
    radialGradient.setColorAt(1,Qt::transparent);

    opacity->setOpacityMask(radialGradient);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void Terrain::fadeBottomRightOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();

    QRadialGradient radialGradient;
    radialGradient.setCenter(0, 0);
    radialGradient.setFocalPoint(radialGradient.center());
    radialGradient.setRadius(pItem->boundingRect().width());
    radialGradient.setColorAt(0,Qt::black);
    radialGradient.setColorAt(0.8,Qt::black);
    radialGradient.setColorAt(1,Qt::transparent);

    opacity->setOpacityMask(radialGradient);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}
