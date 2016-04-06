#include "AsyncShortestPathFinder.h"
#include <QDebug>

APFWorker::APFWorker(PathingMap pathingMap, QPointF start, QPointF end):
    pathingMap_(pathingMap),
    start_(start),
    end_(end)
{

}

void APFWorker::doWork(){
    std::vector<QPointF> result = pathingMap_.shortestPath(start_,end_);
    emit resultReady(result);
    QThread::currentThread()->quit();
}

AsyncShortestPathFinder::AsyncShortestPathFinder(const PathingMap &pathingMap, const QPointF &start, const QPointF &end)
{
    workerThread_ = new QThread();
    worker_ = new APFWorker(pathingMap,start,end);
    worker_->moveToThread(workerThread_);
    connect(worker_,&APFWorker::resultReady,this,&AsyncShortestPathFinder::pathFound_);
    connect(this,&AsyncShortestPathFinder::findPath,worker_,&APFWorker::doWork);
    workerThread_->start();

    // when worker is done, delete him
    connect(workerThread_,&QThread::finished,worker_,&QObject::deleteLater);
}

AsyncShortestPathFinder::~AsyncShortestPathFinder()
{
    workerThread_->quit();
    workerThread_->wait();
}

void AsyncShortestPathFinder::pathFound_(std::vector<QPointF> path)
{
    workerThread_->quit();
    emit pathFound(path);
}
