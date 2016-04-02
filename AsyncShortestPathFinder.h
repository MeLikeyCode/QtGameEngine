#ifndef ASYNCSHORTESTPATHFINDER_H
#define ASYNCSHORTESTPATHFINDER_H

#include <QObject>
#include "PathingMap.h"
#include "Node.h"
#include <QThread>
#include <QDebug>

class AsyncShortestPathFinder;

// private helper class
class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const PathingMap &parameter, const Node& start, const Node& end) {
        std::vector<Node> result;
        /* ... here is the expensive or blocking operation ... */
        result.push_back(Node(0,0));
        emit resultReady(result);
    }

signals:
    void resultReady(const std::vector<Node> &result);
};

// private helper class
class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller(AsyncShortestPathFinder* owner) {
        owner_ = owner;
        Worker *worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }
public slots:
    void handleResults(const std::vector<Node> &result);
signals:
    void operate(const PathingMap &parameter, const Node& start, const Node& end);
private:
    AsyncShortestPathFinder* owner_;
};

/// Represents an object that can asynchronously find the shortest path between
/// two nodes.
class AsyncShortestPathFinder : public QObject
{
    Q_OBJECT
    friend class Controller;
public:
    AsyncShortestPathFinder(const PathingMap& pathingMap, const Node& start, const Node& end);
    std::vector<Node> path();

signals:
    void pathFound();
public slots:

private:
    std::vector<Node> path_;
    Controller* controller_;
};

#endif // ASYNCSHORTESTPATHFINDER_H
