#include "Grid.h"
#include <QPointF>
#include "Node.h"
#include <cassert>

/// Constructs an empty Grid (no cells).
Grid::Grid():
    numXCells_(0),
    numYCells_(0),
    cellWidth_(0),
    cellHeight_(0),
    width_(0),
    height_(0)
{

}

/// Constructs a Grid with the specified number of cells and the specified cell size.
Grid::Grid(int numXCells, int numYCells, int cellWidth, int cellHeight):
    numXCells_(numXCells),
    numYCells_(numYCells),
    cellWidth_(cellWidth),
    cellHeight_(cellHeight)
{
    // constructor body
    // calculate width and height
    width_ = numXCells * cellWidth;
    height_ = numYCells * cellHeight;

}

/// Returns the position of the specified cell.
/// More specifically, returns the position of the top left corner of the specified cell.
QPointF Grid::posOf(const Node &cell) const{
    // make sure the Node is in the Grid
    assert(cell.x() < numXCells() && cell.y() < numYCells());

    int pointX = cell.x() * cellWidth();
    int pointY = cell.y() * cellHeight();
    return QPointF(pointX,pointY);
}

/// Returns the cell at the specified position.
Node Grid::cellAt(const QPointF &pos) const{
    // make sure the point is in the Grid
    assert(int(pos.x()) < width() && int(pos.y()) << height());

    int nodeX = pos.x() / cellWidth();
    int nodeY = pos.y() / cellHeight();
    return Node(nodeX,nodeY);
}

/// Returns all the cells of the specified column of the Grid.
std::vector<Node> Grid::cellsOfColumn(int i) const{
    // make sure the column is in the grid
    assert (i < numXCells());

    std::vector<Node> nodesOfC;
    for (int y = 0, n = numYCells(); y < n; ++y ){
        nodesOfC.push_back(Node(i,y));
    }

    return nodesOfC;
}

/// Returns all the cells of the specified row of the Grid.
std::vector<Node> Grid::cellsOfRow(int i) const{
    // make sure the row is in the grid
    assert(i < numYCells());

    std::vector<Node> nodesOfR;
    for (int x = 0, n = numXCells(); x < n; ++x ){
        nodesOfR.push_back(Node(x,i));
    }

    return nodesOfR;
}

/// Returns all the cells of the Grid.
std::vector<Node> Grid::cells() const{
    std::vector<Node> allNodes;
    // add all columns
    for (int x = 0, n = numXCells(); x < n; ++x){
        std::vector<Node> colNodes = cellsOfColumn(x);
        for (Node node:colNodes){
            allNodes.push_back(node);
        }
    }

    return allNodes;
}

/// Returns the positions of all the cells of the specified column.
/// Their top left position to be more specified.
std::vector<QPointF> Grid::pointsOfColumn(int i) const{
    // get nodes of column
    std::vector<Node> nodesOfC = cellsOfColumn(i);
    std::vector<QPointF> pointsOfC;
    for (Node node:nodesOfC){
        pointsOfC.push_back(posOf(node));
    }

    return pointsOfC;
}

/// Returns the positions of all the cells of the specified row.
/// Their top left positions to be more specific.
std::vector<QPointF> Grid::pointsOfRow(int i) const{
    // get nodes of row
    std::vector<Node> nodesOfR = cellsOfRow(i);
    std::vector<QPointF> pointsOfR;
    for (Node node:nodesOfR){
        pointsOfR.push_back(posOf(node));
    }

    return pointsOfR;
}

/// Returns the positions of all the cells.
/// Their top left positions to be more specific.
std::vector<QPointF> Grid::points() const{
    // get nodes
    std::vector<Node> allNodes = cells();
    std::vector<QPointF> allPoints;
    for (Node node:allNodes){
        allPoints.push_back(posOf(node));
    }

    return allPoints;
}

/// Returns the number of cells in the x direction.
int Grid::numXCells() const{
    return numXCells_;
}

/// Returns the number of cells in the y direction.
int Grid::numYCells() const{
    return numYCells_;
}

/// Returns the width of the Grid.
/// This is (number of cells in x direction) * (cell width).
int Grid::width() const{
    return width_;
}

/// Returns the height of the Grid.
/// This is (number of cells in y direction) * (cell height).
int Grid::height() const{
    return height_;
}

/// Sets the number of cells in the x direction.
void Grid::setNumXCells(int to)
{
    numXCells_ = to;
}

/// Sets the number of cells in the y direction.
void Grid::setNumYCells(int to)
{
    numYCells_ = to;
}

/// Sets the width of each cell.
void Grid::setCellWidth(int to)
{
    cellWidth_ = to;
}

/// Sets the height of each cell.
void Grid::setCellHeight(int to)
{
    cellHeight_ = to;
}

/// Returns the width of each cell.
int Grid::cellWidth() const{
    return cellWidth_;
}

/// Returns the height of each cell.
int Grid::cellHeight() const{
    return cellHeight_;
}
