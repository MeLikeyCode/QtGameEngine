#include "AsyncShortestPathFinder.h"
#include <QDebug>


AsyncShortestPathFinder::AsyncShortestPathFinder()
{
    worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread,&QThread::finished,worker,&QObject::deleteLater);
    connect(worker,&Worker::resultReady,this,&AsyncShortestPathFinder::pathFound_);
    workerThread.start();
}

void AsyncShortestPathFinder::findPath(const PathingMap& pathingMap, const QPointF& start, const QPointF& end)
{
    worker->doWork(pathingMap,start,end);
}

AsyncShortestPathFinder::~AsyncShortestPathFinder()
{
    workerThread.quit();
    workerThread.wait();
}

void AsyncShortestPathFinder::pathFound_(std::vector<QPointF> path)
{
    emit pathFound(path);
}
