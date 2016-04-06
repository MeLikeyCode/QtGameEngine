#ifndef ASYNCSHORTESTPATHFINDER_H
#define ASYNCSHORTESTPATHFINDER_H

#include <QObject>
#include "PathingMap.h"
#include "Node.h"
#include <QThread>
#include <QDebug>

/// This is a private internal class used by AsyncShortestPathFinder.
/// Do not use.
class APFWorker : public QObject
{
    Q_OBJECT
public:
    APFWorker(PathingMap pathingMap, QPointF start, QPointF end);
public slots:
    void doWork();
signals:
    void resultReady(std::vector<QPointF> result);
private:
    PathingMap pathingMap_;
    QPointF start_;
    QPointF end_;
};

/// Represents an object that can asynchronously find the shortest path between
/// two points.
/// @code
/// // example usage
/// PathingMap p;           // a PathingMap
/// QPointF s(200,200);     // start of the path
/// QPointF e(500,500);     // end of the path
///
/// AsyncShortestPathFinder* pf = new AsynchShortetPathFinder(p,s,e);
/// connect(pf,&AsyncShortestPathFinder::pathFound,yourObject,yourCallBackSlot)
/// pf->findPath();
/// @endcode
class AsyncShortestPathFinder : public QObject
{
    Q_OBJECT
public:
    AsyncShortestPathFinder(const PathingMap& pathingMap, const QPointF& start, const QPointF& end);
    ~AsyncShortestPathFinder();
signals:
    void pathFound(std::vector<QPointF> path);
    void findPath();
public slots:
    void pathFound_(std::vector<QPointF> path); // internal use
private:
    QThread* workerThread_;
    APFWorker* worker_;
};

#endif // ASYNCSHORTESTPATHFINDER_H
