#ifndef ASYNCSHORTESTPATHFINDER_H
#define ASYNCSHORTESTPATHFINDER_H

#include <QObject>
#include "PathingMap.h"
#include <QThread>

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
/// @code
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
    void findPath(const PathingMap &pathingMap, const QPointF &start, const QPointF &end);
    ~AsyncShortestPathFinder();

signals:
    void pathFound(std::vector<QPointF> path);
public slots:
    void pathFound_(std::vector<QPointF> path); // internal use
private:
    Worker worker_;
    QThread workerThread_;
};

#endif // ASYNCSHORTESTPATHFINDER_H
