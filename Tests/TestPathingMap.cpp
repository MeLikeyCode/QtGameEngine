#include "TestPathingMap.h"
#include "../PathingMap.h"
#include <QDebug>
#include <cassert>
#include <algorithm>

using namespace qge;

TestPathingMap::TestPathingMap(){
    // =test construction=
    PathingMap pathingMap(5,5,100);

    // make sure everthing is unfilled initially
    std::vector<Node> allCells0 = pathingMap.cells();
    for (Node cell:allCells0){
        assert(!pathingMap.filled(cell));
    }

    // =test cells(const Node&, const Node&) const=
    std::vector<Node> regionCells = pathingMap.cells(Node(1,1),Node(2,2));
    assert(regionCells.size() == 4);
    assert(std::count(regionCells.begin(),regionCells.end(),Node(1,1)) == 1);
    assert(std::count(regionCells.begin(),regionCells.end(),Node(2,1)) == 1);
    assert(std::count(regionCells.begin(),regionCells.end(),Node(1,2)) == 1);
    assert(std::count(regionCells.begin(),regionCells.end(),Node(2,2)) == 1);

    // =test cells() const=
    std::vector<Node> allCells1 = pathingMap.cells();
    assert(allCells1.size() == 25);

    // TODO: test other overloads of cells()

    // =test cellsAsRects(const Node&, const Node&) const=
    std::vector<QRectF> cellRects = pathingMap.cellsAsRects();
    assert(cellRects.size() == 25);
    assert(cellRects[0].width() == pathingMap.cellSize());
    assert(cellRects[0].height() == pathingMap.cellSize());

    // TODO: test other overloads of cellsAsRects()

    // =test filled(const Node&) const=
    assert(!pathingMap.filled(Node(0,0)));
    pathingMap.fill(Node(0,0));
    assert(pathingMap.filled(Node(0,0)));

    // =test filled(const QPointF&) const=
    assert(!pathingMap.filled(QPointF(150,150)));
    pathingMap.fill(QPointF(150,150));
    assert(pathingMap.filled(QPointF(160,160)));

    // =test shortestPath(const Node&, const Node&) const=
    std::vector<QPointF> pathPoints0 = pathingMap.shortestPath(Node(0,1),Node(2,1));
    assert(pathPoints0.size() == 5);
    assert(std::count(pathPoints0.begin(),pathPoints0.end(),QPointF(0,100)) == 1);
    assert(std::count(pathPoints0.begin(),pathPoints0.end(),QPointF(0,200)) == 1);
    assert(std::count(pathPoints0.begin(),pathPoints0.end(),QPointF(100,200)) == 1);
    assert(std::count(pathPoints0.begin(),pathPoints0.end(),QPointF(200,200)) == 1);
    assert(std::count(pathPoints0.begin(),pathPoints0.end(),QPointF(200,100)) == 1);

    // =test shortestPath(const QPointF&, const QPointF&) const=
    std::vector<QPointF> pathPoints1 = pathingMap.shortestPath(QPointF(50,150),QPointF(210,110));
    assert(pathPoints1.size() == 5);
    assert(std::count(pathPoints1.begin(),pathPoints1.end(),QPointF(0,100)) == 1);
    assert(std::count(pathPoints1.begin(),pathPoints1.end(),QPointF(0,200)) == 1);
    assert(std::count(pathPoints1.begin(),pathPoints1.end(),QPointF(100,200)) == 1);
    assert(std::count(pathPoints1.begin(),pathPoints1.end(),QPointF(200,200)) == 1);
    assert(std::count(pathPoints1.begin(),pathPoints1.end(),QPointF(200,100)) == 1);

    // =test width() const=
    assert(pathingMap.width() == 500);

    // =test height() const=
    assert(pathingMap.height() == 500);

    // =test cellSize() const=
    assert(pathingMap.cellSize() == 100);

    // =test numCellsLong() const=
    assert(pathingMap.numCellsLong() == 5);

    // =test numCellsWide() const=
    assert(pathingMap.numCellsWide() == 5);

    // =test pointToCell(const QPointF&) const=
    Node cell = pathingMap.pointToCell(QPointF(150,150));
    assert(cell == Node(1,1));

    // =test cellToPoint(const Node&) const=
    QPointF point = pathingMap.cellToPoint(Node(1,1));
    assert(point == QPointF(100,100));

    // =test fill(const Node&)=
    pathingMap.fill(Node(4,0));
    assert(pathingMap.filled(Node(4,0)));

    // =test fill(const QPointF)=
    pathingMap.fill(QPointF(450,150));
    assert(pathingMap.filled(Node(4,1)));

    // =test unfill(const Node&)=
    pathingMap.unfill(Node(4,0));
    assert(!pathingMap.filled(Node(4,0)));

    // =test unfill(const QPoint&)=
    pathingMap.unfill(QPointF(410,110));
    assert(!pathingMap.filled(Node(4,1)));

    // =test setFilling(const std::vector<std::vector<int>>&)=
    std::vector<std::vector<int>> vec {
        {1,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,1},
    };
    pathingMap.setFilling(vec);

    // make sure these are filled
    std::vector<Node> filledNodes0 = { Node(0,0), Node(4,4) };
    assert(pathingMap.filled(Node(0,0)));
    assert(pathingMap.filled(Node(4,4)));

    // make sure the rest are unfilled
    for (Node node:pathingMap.cells()){
        // if its not supposed to be filled
        if (std::count(filledNodes0.begin(),filledNodes0.end(),node) == 0){
            // then make sure its not filled...
            assert(!pathingMap.filled(node));
        }
    }

    // =test setFilling(const PathingMap&, const Node&)=
    PathingMap littleMap(2,2,100);
    littleMap.fill(Node(0,0));
    littleMap.fill(Node(1,0));
    littleMap.fill(Node(0,1));
    littleMap.fill(Node(1,1));
    pathingMap.setFilling(Node(1,1),littleMap);

    // make sure these are filled
    std::vector<Node> shouldbeFilled = { Node(0,0), Node(4,4), Node(1,1),
                                       Node(2,1), Node(1,2), Node(2,2) };
    for (Node cell:shouldbeFilled){
        assert(pathingMap.filled(cell));
    }

    // make sure the rest are unfilled
    for (Node node:pathingMap.cells()){
        // if its not supposed to be filled
        if (std::count(shouldbeFilled.begin(),shouldbeFilled.end(),node) == 0){
            // then make sure its not filled...
            assert(!pathingMap.filled(node));
        }
    }

    // TODO: test other overloads of setFilling() and addFilling();

    // =test addFilling(const PathingMap&, const Node&)=


    qDebug() << "class PathingMap passed all tests :)";

}
