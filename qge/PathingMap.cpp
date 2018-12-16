#include "PathingMap.h"

#include "Grid.h"

using namespace qge;

/// Constructs a PathingMap with no cells.
PathingMap::PathingMap():
    numCellsWide_(0),
    numCellsLong_(0),
    cellSize_(0),
    pathGrid_()
{

}

/// Constructs a PathingMap with the specified number of cells wide/long and
/// with the specified cell size. All cells are initially empty.
PathingMap::PathingMap( int numCellsWide,  int numCellsLong,  int cellSize):
    pathGrid_(numCellsWide,numCellsLong),
    numCellsWide_(numCellsWide),
    numCellsLong_(numCellsLong),
    cellSize_(cellSize)
{
    // constructor code goes here
}

/// Constructs a pathing map from a painted image.
/// Fully transparent pixels count as "free" areas while any non fully transparent pixels
/// count as "filled" areas.
PathingMap::PathingMap(const QPixmap& pixmap, int cellSize)
{
    QImage image(pixmap.toImage());
    int imageWidth = image.width();
    int imageHeight = image.height();
    numCellsWide_ = imageWidth/cellSize;
    numCellsLong_ = imageHeight/cellSize;
    cellSize_ = cellSize;

    Grid grid(numCellsWide_,numCellsLong_,cellSize,cellSize);

    pathGrid_ = PathGrid(numCellsWide_,numCellsLong_);

    for (int y = 0; y < imageHeight; y+=8){
        for (int x = 0; x < imageWidth; x+=8){
            int alpha = qAlpha(image.pixel(x,y));
            if (alpha != 0)
                fill(grid.pointToCell(QPointF(x,y)));
        }
    }

}

/// Returns a vector of Nodes representing the cells enclosed by the rectangular
/// region defined by a top left node and a bottom right node.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<Node> PathingMap::cells(const Node &topLeft, const Node &bottomRight) const{
    return pathGrid_.nodes(topLeft,bottomRight);
}

/// Returns a vector of Nodes representing the cells enclosed by the rectangular
/// region defined by the top left point and the bottom right point.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<Node> PathingMap::cells(const QPointF &topLeft, const QPointF &bottomRight) const{
    return cells(pointToCell(topLeft),pointToCell(bottomRight));
}

/// Returns a vector of Nodes representing the cells enclosed by the rectangular
/// region.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<Node> PathingMap::cells(const QRectF &inRegion) const{
    return cells(inRegion.topLeft(),inRegion.bottomRight());
}

/// Returns a vector of QRects representing the cells enclosed by the rectangular
/// region defined by a top left node and a bottom right node.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<QRectF> PathingMap::cellsAsRects(const Node &topLeft, const Node &bottomRight) const{
    // get the cells as nodes
    std::vector<Node> cellsAsNodes = cells(topLeft,bottomRight);

    // convert them to rects
    std::vector<QRectF> cellsAsRects;
    int s = cellSize();
    for (Node node:cellsAsNodes){
        cellsAsRects.push_back(QRectF(cellToPoint(node),QSizeF(s-1,s-1)));
    }

    return cellsAsRects;
}

/// Returns a vector of QRects representing the cells enclosed by the rectangular
/// region defined by a top left point and a bottom right point.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<QRectF> PathingMap::cellsAsRects(const QPointF &topLeft, const QPointF &bottomRight) const{
    return cellsAsRects(pointToCell(topLeft),pointToCell(bottomRight));
}

/// Returns a vector of QRects representing the cells enclosed by the rectangular
/// region.
///
/// Please use this function carefully, as it can be expensive for large regions.
/// Big O is n^2.
std::vector<QRectF> PathingMap::cellsAsRects(const QRectF &inRegion) const{
    return cellsAsRects(inRegion.topLeft(),inRegion.bottomRight());
}

/// Returns a vector of QRects representing the cells of the _entire_ PathingMap.
/// Please use this function sparingly as it is quite expensive (big O of n^2).
std::vector<QRectF> PathingMap::cellsAsRects() const{
    return cellsAsRects(Node(0,0),Node(numCellsWide()-1,numCellsLong()-1));
}

/// Returns a vector of Nodes representing the cells of the entire PathingMap.
///
/// Please use this function sparingly as it is quite expensive (big O of n^2).
std::vector<Node> PathingMap::cells() const{
    return cells(Node(0,0),Node(numCellsWide()-1,numCellsLong()-1));
}

/// Returns a QRectF representing the specified cell.
QRectF PathingMap::cellAsRect(Node cell) const{
    int rectWidth = cellSize();
    int rectHeight = cellSize();
    int rectX = cell.x() * cellSize();
    int rectY = cell.y() * cellSize();
    return QRectF(rectX,rectY,rectWidth,rectHeight);
}

/// Returns true if the specified cell is filled.
bool PathingMap::filled(const Node &cell) const{
    return pathGrid_.filled(cell);
}

/// Returns true if the cell at the specified point is filled.
///
/// Overload of PathingMap::filled(const Node&).
bool PathingMap::filled(const QPointF &point) const{
    return filled(pointToCell(point));
}

/// Returns true if all the cells intersecting with the region are filled.
bool PathingMap::filled(const QRectF &region) const
{
    bool result = true;
    // get all the cells in the region
    for (Node cell:cells(region)){
        if (!filled(cell)){
            result = false;
            break;
        }
    }
    return result;
}

/// Returns true if *all* the cells intersectiong the region are free (not filled).
bool PathingMap::free(const QRectF &region) const
{
    bool result = true;
    for (Node cell:cells(region)){
        if (filled(cell)){
            result = false;
            break;
        }
    }

    return result;
}

/// Returns a vector of points representing a path between the specfieid cells.
std::vector<QPointF> PathingMap::shortestPath(const Node &fromCell, const Node &toCell) const{
    // get path from pathGrid_
    std::vector<Node> path = pathGrid_.shortestPath(fromCell,toCell);

    // scale them up into points
    std::vector<QPointF> ptPath;
    for (Node node:path){
        ptPath.push_back(cellToPoint(node));
    }

    return ptPath;
}

/// Returns a vector of points representing a path between the specfieid points.
///
/// Overload of PathingMap::shortestPath(const Node&,const Node&).
std::vector<QPointF> PathingMap::shortestPath(const QPointF &fromPt, const QPointF &toPt) const{
    return shortestPath(pointToCell(fromPt),pointToCell(toPt));
}


/// Returns the width of the map.
int PathingMap::width() const{
    return cellSize_ * numCellsWide_;
}

/// Returns the height of the map.
int PathingMap::height() const{
    return cellSize_ * numCellsLong_;
}

/// Returns the cell size of the map.
int PathingMap::cellSize() const{
    return cellSize_;
}

/// Returns the number of cells in the vertical direction.
int PathingMap::numCellsLong() const{
    return numCellsLong_;
}

/// Returns the number of cells in the horizontal direction.
int PathingMap::numCellsWide() const{
    return numCellsWide_;
}


/// Returns the cell at the specified point.
Node PathingMap::pointToCell(const QPointF &point) const{
    int cellX = point.x() / cellSize();
    int cellY = point.y() / cellSize();
    return Node(cellX,cellY);
}

/// Returns a point representing the top left corner of the specified cell.
QPointF PathingMap::cellToPoint(const Node &cell) const{
    double ptX = cell.x() * cellSize();
    double ptY = cell.y() * cellSize();
    return QPointF(ptX,ptY);
}

/// Returns true if the specified PathingMap can fit in this PathingMap at the
/// specified position.
/// Basically, places the specified PathingMap in this PathingMap at the specified
/// position and sees if it "fits" there without any collision between the filled
/// cells of the two PathingMaps.
bool PathingMap::canFit(const PathingMap &specifiedPathingMap, const QPointF &specifiedPos) const
{
    if (!contains(specifiedPos)){
        return false;
    }

    // get specifiedPathingMap's cells as rects
    std::vector<QRectF> cellsAsRects = specifiedPathingMap.cellsAsRects();

    // get only the filled cells as rects
    std::vector<QRectF> filledCellsAsRects;
    for (QRectF rect:cellsAsRects){
        if (specifiedPathingMap.filled(rect)){
            filledCellsAsRects.push_back(rect);
        }
    }

    // shift them so they're on the specifiedPos on this PathingMap
    std::vector<QRectF> shiftedCells;
    for (QRectF rect:filledCellsAsRects){
        rect.moveTopLeft(QPointF(specifiedPos.x() + rect.x(), specifiedPos.y() + rect.y()));
        shiftedCells.push_back(rect);
    }

    // if any filled cells intersect with the shiftedCells, return false
    for (QRectF shiftedRect:shiftedCells){
        for (Node cell:cells(shiftedRect)){
            if (filled(cell)){
                return false;
            }
        }
    }

    return true;
}

/// Returns if the specified pos is in the PathingMap.
bool PathingMap::contains(const QPointF &pos) const
{
    bool botR = pos.x() < qreal(width()) && pos.y() < qreal(height());
    bool topL = pos.x() > 0 && pos.y() > 0;
    return botR && topL;
}

/// Fills the specified cell.
void PathingMap::fill(const Node &cell){
    // delegate to pathGrid_
    pathGrid_.fill(cell);
}

/// Fills the cell at the specified point.
///
/// Overload of PathingMap::fill(const Node&).
void PathingMap::fill(const QPointF &point){
    fill(pointToCell(point));
}

/// Fills the cells in the specified region.
void PathingMap::fill(const Node &topLeft, const Node &bottomRight){
    // get all the cells
    std::vector<Node> allCells = cells(topLeft,bottomRight);

    // fill them
    for (Node cell:allCells){
        fill(cell);
    }
}

/// Fills the cells in the specified region.
void PathingMap::fill(const QPointF &topLeft, const QPointF &bottomRight){
    fill(pointToCell(topLeft),pointToCell(bottomRight));
}

/// Fills the cells in the specified region.
void PathingMap::fill(const QRectF &region){
    fill(region.topLeft(),region.bottomRight());
}

/// Fills all the cells of the PathingMap.
void PathingMap::fill()
{
    pathGrid_.fill();
}

/// Unfills the specified cell.
void PathingMap::unfill(const Node &cell){
    pathGrid_.unfill(cell);
}

/// Unfills the cell at the specified point.
///
/// Overload of PathingMap::unfill(const Node&).
void PathingMap::unfill(const QPointF &point){
    unfill(pointToCell(point));
}

/// Unfills the specified region.
void PathingMap::unfill(const Node &topLeft, const Node &bottomRight){
    // get the cells
    std::vector<Node> allCells = cells(topLeft,bottomRight);

    // unfill them
    for (Node cell:allCells){
        unfill(cell);
    }

}

/// Overload of PathingMap::unfill(const Node&, const Node&).
void PathingMap::unfill(const QPointF &topLeft, const QPointF &bottomRight){
    unfill(pointToCell(topLeft),pointToCell(bottomRight));
}

/// Overload of PathingMap::unfill(const QPointF&, const QPointF&).
void PathingMap::unfill(const QRectF &region){
    unfill(region.topLeft(),region.bottomRight());
}

/// Unfills all the cells of the PathingMap.
void PathingMap::unfill()
{
    pathGrid_.unfill();
}

/// Fill/unfill cells based on values of a 2d vector.
///
/// A value of 0 means unfilled, anything else means fill.
void PathingMap::setFilling(const std::vector<std::vector<int> > &vec){
    pathGrid_.setFilling(vec);
}

/// Sets the filling of the specified region to be the same as the filling
/// of the specified PathingMap.
///
/// This works best when the two PathingMaps have the same cell sizes. If the
/// cell sizes are different, the resulting pathing is a little inaccurate.
void PathingMap::setFilling(const Node &pos, const PathingMap &littleMap){

    // set the whole region as unfilled at first
    QRectF unfillRegion;
    unfillRegion.setTopLeft(cellToPoint(pos));
    unfillRegion.setWidth(littleMap.width());
    unfillRegion.setHeight(littleMap.height());
    for (Node cell:cells(unfillRegion)){
        unfill(cell);
    }

    // get all of the filled cells of the littleMap as QRectFs
    std::vector<QRectF> filledLittleRects;
    std::vector<Node> littleCells = littleMap.cells();
    for (Node cell:littleCells){
        if (littleMap.filled(cell)){
            QRectF rect = littleMap.cellAsRect(cell);
            //shift it
            int xShiftAmount = pos.x() * cellSize();
            int yShiftAmount = pos.y() * cellSize();
            QPointF shiftedTopLeft(rect.x()+xShiftAmount,rect.y()+yShiftAmount);
            rect.moveTopLeft(shiftedTopLeft);
            filledLittleRects.push_back(rect);
        }
    }

    // fill the necessary ones
    for (QRectF rect:filledLittleRects){
        std::vector<Node> intersectedCells = cells(rect);
        for (Node cell:intersectedCells){
            fill(cell);
        }
    }

}

/// Overload of PathingMap::setFilling(const Node&, const PathingMap&).
void PathingMap::setFilling(const QPointF &pos, const PathingMap &littleMap){
    setFilling(pointToCell(pos),littleMap);
}

/// Adds the specified PathingMap to this PathingMap at the specified pos.
///
/// Adding a pathing map "blends" the two PathingMap's filled
/// regions. In other words, the resulting PathingMap can be more filled but never
/// less filled.
void PathingMap::addFilling(const PathingMap &littleMap, const Node &pos){
    // get all the filled cells of the little PathingMap as QRectF that are
    // shifted by pos.x and pos.y
    QPointF posAsPoint = cellToPoint(pos);
    std::vector<QRectF> littleRects = littleMap.cellsAsRects();
    std::vector<QRectF> filledLittleRects;
    for (QRectF rect:littleRects){
        // if its filled
        if (littleMap.filled(rect.topLeft())){
            // shift it and add it
            rect.moveTopLeft(QPointF(posAsPoint.x()+rect.x(),posAsPoint.y()+rect.y()));
            filledLittleRects.push_back(rect);
        }
    }

    // fill any cells that intersect with these rects
    for (QRectF rect:filledLittleRects){
        // get all the cells it intersects
        std::vector<Node> intersectedCells = cells(rect);
        for (Node cell:intersectedCells){
            // mark the intersected cells as filled
            fill(cell);
        }
    }
}

/// Overload of PathingMap::addFilling(const PathingMap&, const Node&).
void PathingMap::addFilling(const PathingMap &littleMap, const QPointF &pos){
    addFilling(littleMap,pointToCell(pos));
}
