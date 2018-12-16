#pragma once

#include "Vendor.h"

#include "PathingMap.h"

namespace qge{

// private helper class
class Worker : public QObject
{
    Q_OBJECT
public slots:
    void findPath(const PathingMap &pathingMap, const QPointF& start, const QPointF& end);

signals:
    void pathFound(std::vector<QPointF> result);
};

/// Represents an object that can asynchronously find the shortest path between
/// two points.
///
/// Example usage:
/// // create an instance
/// AsyncShortestPathFinder* pathFinder = new AsyncShortestPathFinder(/*...args...*/);
///
/// // connect to pathFound signal
/// connect(pathFinder, &AsyncShortestPathFinder::pathFound, yourObject,yourCallback);
///
/// // start calculating path (your callback will be called with the path when done)
/// pathFinder->findPath(aPathingMap,startPoint,endPoint);
class AsyncShortestPathFinder : public QObject
{
    Q_OBJECT
public:
    AsyncShortestPathFinder();
    ~AsyncShortestPathFinder();

signals:
    void pathFound(std::vector<QPointF> path);
    void findPath(const PathingMap &pathingMap, const QPointF &start, const QPointF &end);
public slots:
    void pathFound_(std::vector<QPointF> path); // internal use
private:
    Worker worker_;
    QThread workerThread_;
};

}
