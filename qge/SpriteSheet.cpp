#include "SpriteSheet.h"

#include "Node.h"

using namespace qge;

/// Constructs a SpriteSheet from the specified file and with the specified number/sized tiles.
SpriteSheet::SpriteSheet(std::string filepath, int numXTiles, int numYTiles, double tileWidth, double tileHeight):
    grid_(numXTiles,numYTiles,tileWidth,tileHeight)
{
    pixmap_.load(QString::fromStdString(filepath));
}

/// Returns the tile at the specified cell in the SpriteSheet.
QPixmap SpriteSheet::tileAt(const Node &cell) const
{
    QRect tileRegion;
    tileRegion.setX(cell.x() * tileWidth());
    tileRegion.setY(cell.y() * tileHeight());
    tileRegion.setWidth(tileWidth());
    tileRegion.setHeight(tileHeight());

    return pixmap_.copy(tileRegion);
}

/// Returns all the tiles in the specified region in the SpriteSheet.
/// The region is [topLeft, bottomRight].
std::vector<QPixmap> SpriteSheet::tilesAt(const Node &topLeft, const Node &bottomRight) const
{
    std::vector<Node> cells = grid_.cells(topLeft, bottomRight);
    std::vector<QPixmap> tiles;
    for (Node cell:cells){
        tiles.push_back(tileAt(cell));
    }
    return tiles;
}

/// Return the number of tiles in the x direction in the SpriteSheet.
int SpriteSheet::numXTiles() const
{
    return grid_.numXCells();
}

/// Return the number of tiles in the y direction in the SpriteSheet.
int SpriteSheet::numYTiles() const
{
    return grid_.numYCells();
}

/// Return the width of each tile in the SpriteSheet.
double SpriteSheet::tileWidth() const
{
    return grid_.cellWidth();
}

/// Return the height of each tile in the SpriteSheet.
double SpriteSheet::tileHeight() const
{
    return grid_.cellHeight();
}
