#include "AsyncShortestPathFinder.h"

using namespace qge;

/// Calculates a Path from a starting point to an ending point in the specified
/// PathingMap.
void Worker::findPath(const PathingMap &pathingMap, const QPointF &start, const QPointF &end){
    std::vector<QPointF> pathPoints;
    pathPoints = pathingMap.shortestPath(start,end);
    emit pathFound(pathPoints);
}

AsyncShortestPathFinder::AsyncShortestPathFinder()
{
    worker_.moveToThread(&workerThread_);
    connect(&worker_,&Worker::pathFound,this,&AsyncShortestPathFinder::pathFound_);
    connect(this,&AsyncShortestPathFinder::findPath,&worker_,&Worker::findPath);
    workerThread_.start();
}

AsyncShortestPathFinder::~AsyncShortestPathFinder()
{
    workerThread_.quit();
    workerThread_.wait(); // block this thread, until workerThread is finished
                         // (finished quitting in this case)
}

/// Worker thread will execute this function when path is found.
/// AsynchShortestPathFinder will in turn emit an event so all listeners
/// get the path.
void AsyncShortestPathFinder::pathFound_(std::vector<QPointF> path)
{
    emit pathFound(path);
}

