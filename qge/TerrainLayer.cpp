#include "TerrainLayer.h"

#include "Node.h"

using namespace qge;

void TerrainLayer::setAlphaChannel(QPixmap& pixmap, int const alpha)
{
  QImage image(pixmap.toImage().convertToFormat(QImage::Format_ARGB32));

  for (int x(0); x != image.width(); ++x)
  {
    for (int y(0); y != image.height(); ++y)
    {
      QColor color(image.pixel(x,y));

      color.setAlpha(alpha);

      image.setPixel(x, y, color.rgba());
    }
  }

  pixmap = QPixmap::fromImage(image);
}

TerrainLayer::TerrainLayer(int numXTiles, int numYTiles):
    TerrainLayer(numXTiles,numYTiles,QPixmap(":resources/graphics/terrain/grass.png"))
{
    // ctro chaining
}

TerrainLayer::TerrainLayer(int numXTiles, int numYTiles, QPixmap pixmap):
    numXTiles_(numXTiles),
    numYTiles_(numYTiles),
    pixmap_(pixmap),
    parentItem_(new QGraphicsPixmapItem())
{
    tileWidth_ = pixmap.width();
    tileHeight_ = pixmap.height();

    grid_ = Grid(numXTiles_,numYTiles_,tileWidth_,tileHeight_);

    // initialize all pixmaps to null
    for (int i = 0, n = numXTiles_* numYTiles_; i < n; i++){
        tiles_.push_back(nullptr);
    }

    linearFadePercent_ = 0.2; // 20%

    fill();
}

/// Sets the position of the TerrainLayer within a Map.
/// This sets the position of the top left corner of the TerrainLayer to the
/// specified pos in the Map.
void TerrainLayer::setPos(const QPointF &pos)
{
    parentItem_->setPos(pos);
}

/// Returns the position of the TerrainLayer.
/// See TerrainLayer::setPos(const QPointF&) for more info.
QPointF TerrainLayer::pos()
{
    return pos_;
}

void TerrainLayer::setPixmap(QPixmap pixmap)
{
    pixmap_ = pixmap;
    draw_();
}

void TerrainLayer::setTileWidth(double width)
{
    tileWidth_ = width;
    grid_ = Grid(numXTiles_,numYTiles_,tileWidth_,tileHeight_); // update grid
    draw_();
}

void TerrainLayer::setTileHeight(double height)
{
    grid_ = Grid(numXTiles_,numYTiles_,tileWidth_,tileHeight_); // update grid
    tileHeight_ = height;
    draw_();
}

/// Fills the specified cell.
void TerrainLayer::fill(const Node &cell){
    QGraphicsPixmapItem* pixmap = pixmapAt_(cell.x(),cell.y());

    // do nothing if already filled
    if (pixmap != nullptr){
        return;
    }

    // otherwise, mark cell filled
    int posInArray = cell.y() * numXTiles_ + cell.x();
    tiles_[posInArray] = new QGraphicsPixmapItem(parentItem_);

    draw_();
}

// Unfills the specified cell.
void TerrainLayer::unfill(const Node &cell)
{
    int posInArray = cell.y() * numXTiles_ + cell.x();
    QGraphicsPixmapItem* tile = tiles_[posInArray];

    // do nothing if already unfilled
    if (tile == nullptr){
        return;
    }

    delete tile;
    tiles_[posInArray] = nullptr;

    draw_();
}

/// Returns true if the specified cell is filled.
bool TerrainLayer::filled(const Node &cell)
{
    int posInArray = cell.y() * numXTiles_ + cell.x();
    QGraphicsPixmapItem* tile = tiles_[posInArray];

    return tile != nullptr;
}

/// Fills the entire TerrainLayer with its pixmap.
void TerrainLayer::fill(){
    // for each cell (node) in the terrain
    for (Node cell:grid_.cells()){
        // fill it with the specified pixmap
        fill(cell);
    }
}

/// Returns the pixmap at the specified location. If there is none, returns nullptr.
QGraphicsPixmapItem *TerrainLayer::pixmapAt_(int xPos, int yPos)
{
    if (xPos < 0 || xPos >= numXTiles_ || yPos < 0 || yPos >= numYTiles_){
        return nullptr;
    }

    int posInArray = yPos * numXTiles_ + xPos;
    return tiles_[posInArray];
}

/// Returns the position (node) of the specified QGraphicsPixmapItem.
Node TerrainLayer::positionOf_(QGraphicsPixmapItem *pixmapItem)
{
    int indexOfPixmap;
    for (int i = 0, n = tiles_.size(); i < n; i++){
        if (tiles_[i] == pixmapItem){
            indexOfPixmap = i;
            break;
        }
    }

    int yPos = indexOfPixmap / numYTiles_;
    int xPos = indexOfPixmap % numYTiles_;
    return Node(xPos,yPos);
}

/// Executed whenver the specified cell needs to recalculate itsfade.
void TerrainLayer::setCorrectFade_(int x, int y)
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
void TerrainLayer::fadeLeftOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(linearFadePercent_*pItem->boundingRect().width(),0);
    lg.setFinalStop(0,0);
    lg.setColorAt(0,Qt::blue);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void TerrainLayer::fadeRightOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(pItem->boundingRect().width() - linearFadePercent_*pItem->boundingRect().width(),0);
    lg.setFinalStop(pItem->boundingRect().width(),0);
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void TerrainLayer::fadeTopOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(0,linearFadePercent_*pItem->boundingRect().width());
    lg.setFinalStop(0,0);
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void TerrainLayer::fadeBottomOnly_(int x, int y)
{
    QGraphicsPixmapItem* pItem = pixmapAt_(x,y);

    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect();
    QLinearGradient lg;
    lg.setStart(0,pItem->boundingRect().height()-linearFadePercent_*pItem->boundingRect().width());
    lg.setFinalStop(0,pItem->boundingRect().height());
    lg.setColorAt(0,Qt::black);
    lg.setColorAt(1,Qt::transparent);
    opacity->setOpacityMask(lg);
    opacity->setOpacity(1);

    pItem->setGraphicsEffect(opacity);
}

void TerrainLayer::fadeTopLeftOnly_(int x, int y)
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

void TerrainLayer::fadeTopRightOnly_(int x, int y)
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

void TerrainLayer::fadeBottomLeftOnly_(int x, int y)
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

void TerrainLayer::fadeBottomRightOnly_(int x, int y)
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

/// Looks at the tile size, current pixmap, and which tiles are filled, then
/// draws everything with the correct fading.
void TerrainLayer::draw_()
{
    // scale the pixmap
    pixmap_ = pixmap_.scaled(tileWidth_,tileHeight_);

    // set alpha
    // setAlphaChannel(pixmap_,254); TODO: why was this here? why is it disabled now?

    // for each filled cell
    // - give the pixmapItem the correct pixmap
    // - give the pixmapItem the correct location (using grid_)
    // - set its fading
    for (QGraphicsPixmapItem* pixmapItem:tiles_){
        if (pixmapItem){
            Node pos = positionOf_(pixmapItem);
            pixmapItem->setPixmap(pixmap_);
            pixmapItem->setPos(grid_.cellToPoint(pos));
            setCorrectFade_(pos.x(),pos.y());
        }
    }
}
