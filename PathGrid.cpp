#include "PathGrid.h"
#include <cassert>
#include <QDebug>
#include "Tree.h"

/// Constructs a PathGrid with the specified number of columns and rows.
///
/// All the Nodes are initially unfilled.
PathGrid::PathGrid( int numCols,  int numRows):
    numCols_(numCols),
    numRows_(numRows)
{
    // constructor body

    // make sure the number or rows/columns is positive
    assert((numCols >= 0) && (numRows >= 0));

    // populate with nodes
    // for each column
    for (size_t i = 0; i < numCols; i++){
        // push back a vector of Nodes
        std::vector<MyNode> colOfNodes;
        for (size_t j = 0; j < numRows; j++){
            MyNode node(i,j);
            colOfNodes.push_back(node);
        }
        nodes_.push_back(colOfNodes);
    }

    // make all nodes unfilled
    for (MyNode node:nodes()){
        filled_[node] = false;
    }
}

/// Fills the specified Node.
///
/// A PathGrid maintains a mapping of Node:bool determining which Nodes are filled. This member function
/// will mark the specified node as filled.
void PathGrid::fill(const MyNode &node){
    filled_[node] = true;
}

/// Overload of PathGrid::fill(const Node&)
void PathGrid::fill( int x,  int y){
    fill(MyNode(x,y));
}

/// Unfills the specified Node.
///
/// @see PathGrid::fill(const Node&)
void PathGrid::unfill(const MyNode &node){
    filled_[node] = false;
}

/// Overload of PathGrid::unfill(const Node&)
void PathGrid::unfill( int x,  int y){
    unfill(MyNode(x,y));
}

/// Fills/unfills Nodes based on the values of a 2d int vector.
///
/// A value of 0 means that the specfied Node should be unfilled. Any other value
/// means that it should be filled. Please ensure that the 2d vector is the same
/// size as the PathGrid. If it is shorter, the remaining Nodes will be unfilled.
/// If it is longer, you will get an out of range error.=
void PathGrid::setFilling(const std::vector<std::vector<int>>& vec){
    for (int y = 0, n = nodes_[0].size(); y < n; y++){
        for (int x = 0, p = nodes_.size(); x < p; x++){
            if (vec[y][x] == 0){
                unfill(x,y);
            }
            else{
                fill(x,y);
            }
        }
    }
}

/// Sets the filling _starting_ at the specified location to be the same filling
/// as that of the specified PathGrid.
void PathGrid::setFilling(const PathGrid &pathGrid, const MyNode &pos){
    // approach:
    // -traverse through Nodes of new PathGrid
    // -set shadowed Node to be the same fillness

    for (MyNode node:pathGrid.nodes()){
        MyNode shadowed(node.x()+pos.x(),node.y()+pos.y());
        // if node is filled, then fill shadowed node
        if (pathGrid.filled(node)){
            fill(shadowed);
        }
        else {
            unfill(shadowed);
        }
    }
}

/// Adds the specified PathGrid to this PathGrid at the specified positino.
///
/// The resulting PathGrid can be more filled but not less filled.
void PathGrid::addPathGrid(const PathGrid &pathGrid, const MyNode &pos){
    // approach
    // -traverse through Nodes of new path
    // -if Node is filled, make sure shadowed is filled too

    for (MyNode node:pathGrid.nodes()){
        MyNode shadowed(node.x()+pos.x(),node.y()+pos.y());
        // if node is filled and shadowed node is not
        if (pathGrid.filled(node)){
            // fill shadowed node
            fill(shadowed);
        }
    }
}

/// Returns a vector of all the Nodes of the PathGrid.
std::vector<MyNode> PathGrid::nodes() const{
    return nodes(MyNode(0,0),MyNode(numCols_-1,numRows_-1));
}

/// Returns true if the specified Node is filled.
bool PathGrid::filled(const MyNode &node) const{
    // make sure the node is actually in the PathGrid
    assert(contains(node));

    return filled_.find(node)->second;
}

/// Overload of PathGrid::filled(const Node&).
bool PathGrid::filled( int x,  int y) const{
    return filled(MyNode(x,y));
}

/// Returns a vector of all the adjacent unfilled neighboring Nodes of the specified node.
///
/// This represents all the diretions in which one could move from the specfied Node.
std::vector<MyNode> PathGrid::unfilledNeighbors(const MyNode &of) const{
    int numCols = nodes_.size();
    int numRows = nodes_[0].size();

    std::vector<MyNode> neighbors;

    // up
    if (of.y() > 0){
        MyNode upNode(of.x(),of.y()-1);
        if (!filled(upNode)){
            neighbors.push_back(upNode);
        }
    }

    // down
    if (of.y() < numRows - 1){
        MyNode downNode(of.x(),of.y()+1);
        if (!filled(downNode)){
            neighbors.push_back(downNode);
        }
    }

    // left
    if (of.x() > 0){
        MyNode leftNode(of.x()-1,of.y());
        if (!filled(leftNode)){
            neighbors.push_back(leftNode);
        }
    }

    // right
    if (of.x() < numCols - 1){
        MyNode rightNode(of.x()+1,of.y());
        if (!filled(rightNode)){
            neighbors.push_back(rightNode);
        }
    }

    return neighbors;

}

/// Returns a Graph representation of the PathGrid.
///
/// More specifically, returns a Graph containing all the _unfilled_ Nodes and the Edges between them.
Graph PathGrid::toGraph() const{
    Graph g;

    // add all nodes
    for (MyNode node:nodes()){
        g.addNode(node);
    }

    // add all edges
    for (MyNode node:g.nodes()){
        // skip if the node is filled
        if (filled(node)){
            continue;
        }

        // get all the neighbors of the node
        std::vector<MyNode> neighbors = unfilledNeighbors(node);
        for (MyNode neighbor:neighbors){
            g.addEdge(node,neighbor,1);
            g.addEdge(neighbor,node,1);
        }
    }

    return g;
}

/// Returns a vector of Nodes that represents the shortest path between the specified Nodes.
std::vector<MyNode> PathGrid::shortestPath(const MyNode &from, const MyNode &to) const{
    return toGraph().shortestPath(from,to);
}

/// Returns a vector of all the Nodes in the i-eth column.
std::vector<MyNode> PathGrid::column( int i) const{
    return nodes(MyNode(i,0),MyNode(i,numRows_-1));
}

/// Returns a vector of all the Nodes in the i-eth row.
std::vector<MyNode> PathGrid::row(int i) const{
    return nodes(MyNode(0,i),MyNode(numCols_-1,i));
}

/// Returns a vector of Nodes enclosed by the rectangular region defined by a top
/// left Node and a bottom right Node. Both Nodes are inclusive.
std::vector<MyNode> PathGrid::nodes(const MyNode &topLeft, const MyNode &bottomRight) const{
    // make sure the region is in the PathGrid
    assert(contains(topLeft) && contains(bottomRight));

    std::vector<MyNode> nodesInRegion;
    for (int x = topLeft.x(), n = bottomRight.x(); x <= n; ++x){
        for (int y = topLeft.y(), p = bottomRight.y(); y <= p; ++y){
            nodesInRegion.push_back(MyNode(x,y));
        }
    }

    return nodesInRegion;

}

/// Returns true if the PathGrid contains the specified Node.
bool PathGrid::contains(const MyNode &node) const{
    // if the node's x and y values are less than the width and height, its in
    return (node.x() < numCols_ && node.y() < numRows_);
}

/// Returns the number of columns in the PathGrid.
int PathGrid::numCols() const{
    return numCols_;
}

/// Returns the number of rows in the PathGrid.
int PathGrid::numRows() const{
    return numRows_;
}
