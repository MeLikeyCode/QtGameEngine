#include "TestPathGrid.h"
#include "../PathGrid.h"
#include <QDebug>
#include <cassert>
#include <algorithm>

using namespace qge;

TestPathGrid::TestPathGrid(){
    // create a PathGrid
    PathGrid pathGrid(5,5);

    // =test filled(const Node&) const=
    assert(!pathGrid.filled(Node(0,0)));
    pathGrid.fill(1,1);
    assert(pathGrid.filled(Node(1,1)));

    //=test filled(int x, int y) const=
    assert(!pathGrid.filled(4,4));
    assert(pathGrid.filled(1,1));

    // =test unfilledNeighbors(const Node&) const=
    std::vector<Node> unfilledN = pathGrid.unfilledNeighbors(Node(0,0));
    assert(unfilledN.size() == 2);
    assert(std::count(unfilledN.begin(),unfilledN.end(),Node(1,0)) == 1);
    assert(std::count(unfilledN.begin(),unfilledN.end(),Node(0,1)) == 1);

    // =test shortestPath(const Node&,const Node&) const=
    pathGrid.fill(2,1);
    pathGrid.fill(3,1);
    std::vector<Node> shortPath = pathGrid.shortestPath(Node(0,0),Node(1,2));
    assert(shortPath.size() == 4);
    assert(std::count(shortPath.begin(),shortPath.end(),Node(0,0)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),Node(0,1)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),Node(0,2)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),Node(1,2)) == 1);

    // =test nodes(const Node&, const Node&) const=
    std::vector<Node> regionNodes = pathGrid.nodes(Node(1,0),Node(2,1));
    assert(regionNodes.size() == 4);
    assert(std::count(regionNodes.begin(),regionNodes.end(),Node(1,0)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),Node(2,0)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),Node(1,1)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),Node(2,1)));

    // =test column(int) const=
    std::vector<Node> rowNodes = pathGrid.row(2);
    assert(rowNodes.size() == 5);
    assert(std::count(rowNodes.begin(),rowNodes.end(),Node(0,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),Node(1,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),Node(2,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),Node(3,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),Node(4,2)) == 1);

    // =test row(int) const=
    std::vector<Node> columnNodes = pathGrid.column(3);
    assert(columnNodes.size() == 5);
    assert(std::count(columnNodes.begin(),columnNodes.end(),Node(3,0)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),Node(3,1)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),Node(3,2)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),Node(3,3)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),Node(3,4)));

    // =test nodes() const=
    std::vector<Node> allNodes = pathGrid.nodes();
    assert(allNodes.size() == 25);
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            assert(std::count(allNodes.begin(),allNodes.end(),Node(x,y)) == 1);
        }
    }

    // =test contains(const Node&)=
    assert(pathGrid.contains(Node(0,0)));
    assert(!pathGrid.contains(Node(6,6)));

    // =test numCols() const=
    assert(pathGrid.numCols() == 5);

    // =test numRows() const=
    assert(pathGrid.numRows() == 5);

    // =test fill(const Node&)=
    assert(!pathGrid.filled(0,0));
    pathGrid.fill(Node(0,0));
    assert(pathGrid.filled(0,0));

    // =test fill(int,int)=
    pathGrid.fill(0,4);
    assert(pathGrid.filled(0,4));

    // =test unfill(const Node&)=
    assert(pathGrid.filled(0,0));
    pathGrid.unfill(Node(0,0));
    assert(!pathGrid.filled(0,0));

    // =test unfill(int,int)=
    pathGrid.fill(0,0);
    assert(pathGrid.filled(0,0));
    pathGrid.unfill(0,0);
    assert(!pathGrid.filled(0,0));

    // =test setFilling(const std::vector<std::vector<int>>&)=
    std::vector<std::vector<int>> fillVec = {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1}
    };
    pathGrid.setFilling(fillVec);

    // make sure these are filled
    std::vector<Node> shouldbeFilled0 = { Node(0,0), Node(4,4)};
    for (Node node:shouldbeFilled0){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            Node nodeToCheck(x,y);
            // if the node shouldn't be filled
            if (std::count(shouldbeFilled0.begin(),shouldbeFilled0.end(),nodeToCheck) == 0){
                // make sure it isn't...
                assert(!pathGrid.filled(nodeToCheck));
            }
        }
    }

    // =test setFilling(const PathGrid&, const Node&)=
    PathGrid smallGrid0(2,2);
    smallGrid0.fill(0,0);
    smallGrid0.fill(1,1);
    pathGrid.setFilling(smallGrid0,Node(1,1));

    // make sure these are filled
    std::vector<Node> shouldbeFilled1 = { Node(0,0), Node(1,1), Node(2,2), Node(4,4)};
    for (Node node:shouldbeFilled1){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            Node nodeToCheck(x,y);
            // if the node shouldn't be filled
            if (std::count(shouldbeFilled1.begin(),shouldbeFilled1.end(),nodeToCheck) == 0){
                // make sure its not...
                assert(!pathGrid.filled(nodeToCheck));
            }
        }
    }

    // =test addPathGrid(const PathGrid&, const Node&)=
    PathGrid smallGrid1(2,2);
    smallGrid1.fill(1,0);
    pathGrid.addPathGrid(smallGrid1,Node(1,1));

    // make sure these are filled
    std::vector<Node> shouldbeFilled2 = {Node(0,0), Node(1,1), Node(2,1), Node(2,2), Node(4,4)};
    for (Node node:shouldbeFilled2){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            Node nodeToCheck(x,y);
            // if the node shouldn't be filled
            if (std::count(shouldbeFilled2.begin(),shouldbeFilled2.end(),nodeToCheck) == 0){
                // make sure its not...
                assert(!pathGrid.filled(nodeToCheck));
            }
        }
    }


    qDebug() << "class PathGrid passed all tests :)";
}
