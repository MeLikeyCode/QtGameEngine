#include "TestPathGrid.h"
#include "../PathGrid.h"
#include <QDebug>
#include <cassert>
#include <algorithm>

TestPathGrid::TestPathGrid(){
    // create a PathGrid
    PathGrid pathGrid(5,5);

    // =test filled(const Node&) const=
    assert(!pathGrid.filled(MyNode(0,0)));
    pathGrid.fill(1,1);
    assert(pathGrid.filled(MyNode(1,1)));

    //=test filled(int x, int y) const=
    assert(!pathGrid.filled(4,4));
    assert(pathGrid.filled(1,1));

    // =test unfilledNeighbors(const Node&) const=
    std::vector<MyNode> unfilledN = pathGrid.unfilledNeighbors(MyNode(0,0));
    assert(unfilledN.size() == 2);
    assert(std::count(unfilledN.begin(),unfilledN.end(),MyNode(1,0)) == 1);
    assert(std::count(unfilledN.begin(),unfilledN.end(),MyNode(0,1)) == 1);

    // =test shortestPath(const Node&,const Node&) const=
    pathGrid.fill(2,1);
    pathGrid.fill(3,1);
    std::vector<MyNode> shortPath = pathGrid.shortestPath(MyNode(0,0),MyNode(1,2));
    assert(shortPath.size() == 4);
    assert(std::count(shortPath.begin(),shortPath.end(),MyNode(0,0)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),MyNode(0,1)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),MyNode(0,2)) == 1);
    assert(std::count(shortPath.begin(),shortPath.end(),MyNode(1,2)) == 1);

    // =test nodes(const Node&, const Node&) const=
    std::vector<MyNode> regionNodes = pathGrid.nodes(MyNode(1,0),MyNode(2,1));
    assert(regionNodes.size() == 4);
    assert(std::count(regionNodes.begin(),regionNodes.end(),MyNode(1,0)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),MyNode(2,0)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),MyNode(1,1)));
    assert(std::count(regionNodes.begin(),regionNodes.end(),MyNode(2,1)));

    // =test column(int) const=
    std::vector<MyNode> rowNodes = pathGrid.row(2);
    assert(rowNodes.size() == 5);
    assert(std::count(rowNodes.begin(),rowNodes.end(),MyNode(0,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),MyNode(1,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),MyNode(2,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),MyNode(3,2)) == 1);
    assert(std::count(rowNodes.begin(),rowNodes.end(),MyNode(4,2)) == 1);

    // =test row(int) const=
    std::vector<MyNode> columnNodes = pathGrid.column(3);
    assert(columnNodes.size() == 5);
    assert(std::count(columnNodes.begin(),columnNodes.end(),MyNode(3,0)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),MyNode(3,1)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),MyNode(3,2)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),MyNode(3,3)));
    assert(std::count(columnNodes.begin(),columnNodes.end(),MyNode(3,4)));

    // =test nodes() const=
    std::vector<MyNode> allNodes = pathGrid.nodes();
    assert(allNodes.size() == 25);
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            assert(std::count(allNodes.begin(),allNodes.end(),MyNode(x,y)) == 1);
        }
    }

    // =test contains(const Node&)=
    assert(pathGrid.contains(MyNode(0,0)));
    assert(!pathGrid.contains(MyNode(6,6)));

    // =test numCols() const=
    assert(pathGrid.numCols() == 5);

    // =test numRows() const=
    assert(pathGrid.numRows() == 5);

    // =test fill(const Node&)=
    assert(!pathGrid.filled(0,0));
    pathGrid.fill(MyNode(0,0));
    assert(pathGrid.filled(0,0));

    // =test fill(int,int)=
    pathGrid.fill(0,4);
    assert(pathGrid.filled(0,4));

    // =test unfill(const Node&)=
    assert(pathGrid.filled(0,0));
    pathGrid.unfill(MyNode(0,0));
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
    std::vector<MyNode> shouldbeFilled0 = { MyNode(0,0), MyNode(4,4)};
    for (MyNode node:shouldbeFilled0){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            MyNode nodeToCheck(x,y);
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
    pathGrid.setFilling(smallGrid0,MyNode(1,1));

    // make sure these are filled
    std::vector<MyNode> shouldbeFilled1 = { MyNode(0,0), MyNode(1,1), MyNode(2,2), MyNode(4,4)};
    for (MyNode node:shouldbeFilled1){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            MyNode nodeToCheck(x,y);
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
    pathGrid.addPathGrid(smallGrid1,MyNode(1,1));

    // make sure these are filled
    std::vector<MyNode> shouldbeFilled2 = {MyNode(0,0), MyNode(1,1), MyNode(2,1), MyNode(2,2), MyNode(4,4)};
    for (MyNode node:shouldbeFilled2){
        assert(pathGrid.filled(node));
    }

    // make sure the rest are unfilled
    for (int x = 0, n = pathGrid.numCols(); x < n; ++x){
        for (int y = 0, p = pathGrid.numRows(); y < p; ++y){
            MyNode nodeToCheck(x,y);
            // if the node shouldn't be filled
            if (std::count(shouldbeFilled2.begin(),shouldbeFilled2.end(),nodeToCheck) == 0){
                // make sure its not...
                assert(!pathGrid.filled(nodeToCheck));
            }
        }
    }


    qDebug() << "class PathGrid passed all tests :)";
}
