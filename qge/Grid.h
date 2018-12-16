#pragma once

#include "Vendor.h"

class QPointF;
class QRectF;

namespace qge{

class Node;

/// Represents a region of 2d space that is divided into a grid of cells.
/// You can do things such finding the cell at a specified point, finding the position
/// of a specified cell, getting a bounding box of a cell or some cells in a region, etc...
/// This class is a value class.
/// @author Abdullah Aghazadah
/// @date 5-15-15
///
/// Here are a few things you can do to a Grid:
/// To get the top left position of a specified cell, use cellToPoint().
/// To get the cell at the specified point, use pointToCell();
/// Functions exist to get the cells in a specified region of the Grid.
/// Functions exist to get the top left corners of a bunch of cells in a specified region.
///
/// The cell (0,0) represents the top left corner of the grid while the cell
/// (numXCells-1,numYCells-1) represents the bottom right of the grid.
class Grid{
public:
    // constructors
    Grid();
    Grid(int numXCells, int numYCells, int cellWidth, int cellHeight);

    // readers
    bool contains(const Node& cell) const;
    QPointF cellToPoint(const Node& cell) const;
    Node pointToCell(const QPointF& pos) const;
    QRectF cellToRect(const Node& cell) const;
    std::vector<Node> cellsOfColumn(int i) const;
    std::vector<Node> cellsOfRow(int i) const;
    std::vector<Node> cells() const;
    std::vector<Node> cells(const Node& topLeft, const Node& bottomRight) const;
    std::vector<Node> cellsIn(const QRectF& region) const;
    std::vector<Node> cellsFullyIn(const QRectF& region) const;
    std::vector<QPointF> pointsOfColumn(int i) const;
    std::vector<QPointF> pointsOfRow(int i) const;
    std::vector<QPointF> points() const;
    std::vector<QRectF> cellsToRects(const Node& topLeft, const Node& bottomRight) const;

    int numXCells() const;
    int numYCells() const;
    int cellWidth() const;
    int cellHeight() const;
    int width() const;
    int height() const;

    // setters
    void setNumXCells(int to);
    void setNumYCells(int to);
    void setCellWidth(int to);
    void setCellHeight(int to);

private:
    int numXCells_;
    int numYCells_;
    int cellWidth_;
    int cellHeight_;
    int width_;
    int height_;
};

}
