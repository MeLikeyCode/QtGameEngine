#ifndef PATHGRID_H
#define PATHGRID_H

#include <vector>
#include "Node.h"
#include <unordered_map>
#include "Graph.h"

/// Represents a grid of Nodes, each of which can either be filled or unfilled.
/// @author Abdullah Aghazadah
/// @date 4-26-15
///
/// The (x,y) value of the top left node is (0,0). The (x,y) value of the bottom
/// right node is (width,height).
class PathGrid{
public:
    // constructors
    PathGrid();
    PathGrid(int numCols,  int numRows);
    PathGrid(const PathGrid& copy) = default;

    // readers ("getters")
    bool filled(const Node& node) const;
    bool filled(int x, int y) const;
    std::vector<Node> unfilledNeighbors(const Node& of) const;
    std::vector<Node> shortestPath(const Node& from, const Node& to) const;
    std::vector<Node> nodes(const Node& topLeft, const Node& bottomRight) const;
    std::vector<Node> column(int i) const;
    std::vector<Node> row(int i) const;
    std::vector<Node> nodes() const;
    bool contains(const Node& node) const;
    int numCols() const;
    int numRows() const;

    // modifiers ("setters")
    void fill(const Node& node);
    void fill(int x, int y);
    void unfill(const Node& node);
    void unfill(int x, int y);
    void setFilling(const std::vector<std::vector<int>>& vec);
    void setFilling(const PathGrid& pathGrid, const Node& pos);
    void addPathGrid(const PathGrid& pathGrid, const Node& pos);

private:
    // main private attributes
    std::vector<std::vector<Node>> nodes_;
    std::unordered_map<Node,bool> filled_;
    int numCols_;
    int numRows_;

    // helper
    Graph toGraph() const;

};

#endif // PATHGRID_H
