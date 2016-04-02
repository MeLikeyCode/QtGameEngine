#include "AsyncShortestPathFinder.h"
#include <QDebug>


AsyncShortestPathFinder::AsyncShortestPathFinder(const PathingMap &pathingMap, const Node &start, const Node &end)
{
    controller_ = new Controller(this);
    controller_->operate();
}

void Controller::handleResults(const std::vector<Node> &result){
    qDebug() << "path found";
    owner_->path_ = result;
    emit owner_->pathFound();
}
