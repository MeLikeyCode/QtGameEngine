#ifndef GRID_H
#define GRID_H

// uses:
class QPointF;
class Node;
#include <vector>

/// Represents a region of space that is divided into Nodes in a grid
/// like fashion.
/// @author Abdullah Aghazadah
/// @date 5-15-15
///
/// To get the actual point at a specified Node, use Grid::toPoint(Node).
/// To get the Node closest to (the top left of) a point, use Grid::toNode(QPointF).
/// To get a vector of all the Nodes, use Grid::nodes();
/// To get a vector of all the Nodes in a specific column or row use
/// Grid::nodesOfColumn(int) or Grid::nodesOfRow(int). Similar functions exist
/// to get the vector of all the points in a specified column, row, or in the
/// entire Grid.
///
/// The Node (0,0) represents the top left corner of the grid while the Node
/// (width,height) represents the bottom right of the grid.
class Grid{
public:
    // constructor
    Grid();
    Grid(int numXNodes, int numYNodes, int nodeWidth, int nodeHeight);

    // readers
    QPointF nodeToPoint(const Node& node) const;
    Node pointToNode(const QPointF& point) const;
    std::vector<Node> nodesOfColumn(int i) const;
    std::vector<Node> nodesOfRow(int i) const;
    std::vector<Node> nodes() const;
    std::vector<QPointF> pointsOfColumn(int i) const;
    std::vector<QPointF> pointsOfRow(int i) const;
    std::vector<QPointF> points() const;
    int numXNodes() const;
    int numYNodes() const;
    int nodeWidth() const;
    int nodeHeight() const;
    int width() const;
    int height() const;

private:
    int numXNodes_;
    int numYNodes_;
    int nodeWidth_;
    int nodeHeight_;
    int width_;
    int height_;
};

#endif // GRID_H
