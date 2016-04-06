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
    PathGrid(int numCols,  int numRows);

    // readers ("getters")
    bool filled(const MyNode& node) const;
    bool filled(int x, int y) const;
    std::vector<MyNode> unfilledNeighbors(const MyNode& of) const;
    std::vector<MyNode> shortestPath(const MyNode& from, const MyNode& to) const;
    std::vector<MyNode> nodes(const MyNode& topLeft, const MyNode& bottomRight) const;
    std::vector<MyNode> column(int i) const;
    std::vector<MyNode> row(int i) const;
    std::vector<MyNode> nodes() const;
    bool contains(const MyNode& node) const;
    int numCols() const;
    int numRows() const;

    // modifiers ("setters")
    void fill(const MyNode& node);
    void fill(int x, int y);
    void unfill(const MyNode& node);
    void unfill(int x, int y);
    void setFilling(const std::vector<std::vector<int>>& vec);
    void setFilling(const PathGrid& pathGrid, const MyNode& pos);
    void addPathGrid(const PathGrid& pathGrid, const MyNode& pos);

private:
    // main private attributes
    std::vector<std::vector<MyNode>> nodes_;
    std::unordered_map<MyNode,bool> filled_;
    int numCols_;
    int numRows_;

    // helper
    Graph toGraph() const;

};

#endif // PATHGRID_H
