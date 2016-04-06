#include "Grid.h"
#include <QPointF>
#include "Node.h"
#include <cassert>

Grid::Grid(int numXNodes, int numYNodes, int nodeWidth, int nodeHeight):
    numXNodes_(numXNodes),
    numYNodes_(numYNodes),
    nodeWidth_(nodeWidth),
    nodeHeight_(nodeHeight)
{
    // constructor body
    // calculate width and height
    width_ = numXNodes * nodeWidth;
    height_ = numYNodes * nodeHeight;

}

QPointF Grid::nodeToPoint(const MyNode &node) const{
    // make sure the Node is in the Grid
    assert(node.x() < numXNodes() && node.y() < numYNodes());

    int pointX = node.x() * nodeWidth();
    int pointY = node.y() * nodeHeight();
    return QPointF(pointX,pointY);
}

/// Returns the Node closest to the top left of the specified point.
MyNode Grid::pointToNode(const QPointF &point) const{
    // make sure the point is in the Grid
    assert(int(point.x()) < width() && int(point.y()) << height());

    int nodeX = point.x() / nodeWidth();
    int nodeY = point.y() / nodeHeight();
    return MyNode(nodeX,nodeY);
}

std::vector<MyNode> Grid::nodesOfColumn(int i) const{
    // make sure the column is in the grid
    assert (i < numXNodes());

    std::vector<MyNode> nodesOfC;
    for (int y = 0, n = numYNodes(); y < n; ++y ){
        nodesOfC.push_back(MyNode(i,y));
    }

    return nodesOfC;
}

std::vector<MyNode> Grid::nodesOfRow(int i) const{
    // make sure the row is in the grid
    assert(i < numYNodes());

    std::vector<MyNode> nodesOfR;
    for (int x = 0, n = numXNodes(); x < n; ++x ){
        nodesOfR.push_back(MyNode(x,i));
    }

    return nodesOfR;
}

std::vector<MyNode> Grid::nodes() const{
    std::vector<MyNode> allNodes;
    // add all columns
    for (int x = 0, n = numXNodes(); x < n; ++x){
        std::vector<MyNode> colNodes = nodesOfColumn(x);
        for (MyNode node:colNodes){
            allNodes.push_back(node);
        }
    }

    return allNodes;
}

std::vector<QPointF> Grid::pointsOfColumn(int i) const{
    // get nodes of column
    std::vector<MyNode> nodesOfC = nodesOfColumn(i);
    std::vector<QPointF> pointsOfC;
    for (MyNode node:nodesOfC){
        pointsOfC.push_back(nodeToPoint(node));
    }

    return pointsOfC;
}

std::vector<QPointF> Grid::pointsOfRow(int i) const{
    // get nodes of row
    std::vector<MyNode> nodesOfR = nodesOfRow(i);
    std::vector<QPointF> pointsOfR;
    for (MyNode node:nodesOfR){
        pointsOfR.push_back(nodeToPoint(node));
    }

    return pointsOfR;
}

std::vector<QPointF> Grid::points() const{
    // get nodes
    std::vector<MyNode> allNodes = nodes();
    std::vector<QPointF> allPoints;
    for (MyNode node:allNodes){
        allPoints.push_back(nodeToPoint(node));
    }

    return allPoints;
}

int Grid::numXNodes() const{
    return numXNodes_;
}

int Grid::numYNodes() const{
    return numYNodes_;
}

int Grid::width() const{
    return width_;
}

int Grid::height() const{
    return height_;
}

int Grid::nodeWidth() const{
    return nodeWidth_;
}

int Grid::nodeHeight() const{
    return nodeHeight_;
}
