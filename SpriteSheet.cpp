#include "SpriteSheet.h"

#include <QPixmap>
#include <QRect>

#include "Node.h"

/// Constructs a SpriteSheet from the specified file and with the specified number/sized tiles.
SpriteSheet::SpriteSheet(std::string filepath, int numXTiles, int numYTiles, double tileWidth, double tileHeight):
    grid_(numXTiles,numYCells,cellWidth,cellHeight)
{
    pixmap_.load(filepath);
}

QPixmap SpriteSheet::tileAt(const Node &cell) const
{
    QRect tileRegion;
    tileRegion.setX(cell.x() * tileWidth());
    tileRegion.setY(cell.y() * tileHeight());
    tileRegion.setWidth(tileWidth());
    tileRegion.setHeight(tileHeight());

    return pixmap_.copy(tileRegion);
}

int SpriteSheet::numXTiles() const
{
    return grid_.numXCells();
}

int SpriteSheet::numYTiles() const
{
    return grid_.numYCells();
}

double SpriteSheet::tileWidth() const
{
    return grid_.cellWidth();
}

double SpriteSheet::tileHeight() const
{
    return grid_.cellHeight();
}
