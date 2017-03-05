#ifndef GRID_H
#define GRID_H

#include <vector>

class QPointF;
class Node;

/// Represents a region of space that is divided into a grid of cells.
/// You can set the the number and size of the cells of the grid.
/// Provides functions for mapping between a cell and an actual position.
/// @author Abdullah Aghazadah
/// @date 5-15-15
///
/// To get the actual point at a specified cell, use posOf().
/// To get the cell at the specified point, use cellAt();
/// To get a vector of all the cells, use cells();
/// To get a vector of all the cells in a specific column or row use
/// cellsOfColumn(int) or cellsOfRow(int). Similar functions exist
/// to get the vector of all the points in a specified column, row, or in the
/// entire Grid.
///
/// The cell (0,0) represents the top left corner of the grid while the cell
/// (numXCells-1,numYCells-1) represents the bottom right of the grid.
class Grid{
public:
    // constructor
    Grid();
    Grid(int numXCells, int numYCells, int cellWidth, int cellHeight);

    // readers
    QPointF posOf(const Node& cell) const;
    Node cellAt(const QPointF& pos) const;
    std::vector<Node> cellsOfColumn(int i) const;
    std::vector<Node> cellsOfRow(int i) const;
    std::vector<Node> cells() const;
    std::vector<QPointF> pointsOfColumn(int i) const;
    std::vector<QPointF> pointsOfRow(int i) const;
    std::vector<QPointF> points() const;
    int numXCells() const;
    int numYCells() const;
    int cellWidth() const;
    int cellHeight() const;
    int width() const;
    int height() const;

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

#endif // GRID_H
