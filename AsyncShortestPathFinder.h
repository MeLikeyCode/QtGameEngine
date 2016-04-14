#ifndef ASYNCSHORTESTPATHFINDER_H
#define ASYNCSHORTESTPATHFINDER_H

#include <QObject>
#include "PathingMap.h"
#include "Node.h"
#include <QThread>
#include <QDebug>

// private helper class
class Worker : public QObject
{
    Q_OBJECT
public slots:
    void doWork(const PathingMap &pathingMap, const QPointF& start, const QPointF& end) {
        std::vector<QPointF> result;
        // ... here is the expensive or blocking operation ...
        result = pathingMap.shortestPath(start,end);
        emit resultReady(result);
    }

signals:
    void resultReady(std::vector<QPointF> result);
};

/// Represents an object that can asynchronously find the shortest path between
/// two points.
/// - create an instance of the object
/// - connect a slot to its pathFound() signal
/// - call findPath(PathingMap,startPoint,endPoint)
class AsyncShortestPathFinder : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    AsyncShortestPathFinder();
    void findPath(const PathingMap &pathingMap, const QPointF &start, const QPointF &end);
    ~AsyncShortestPathFinder();

signals:
    void pathFound(std::vector<QPointF> path);
public slots:
    void pathFound_(std::vector<QPointF> path); // internal use
private:
    Worker* worker;
};

#endif // ASYNCSHORTESTPATHFINDER_H
