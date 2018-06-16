#pragma once

#include <QObject>

class Game;

class EventHandler : public QObject
{
    Q_OBJECT
public:
    EventHandler(Game* forGame);
public slots:
    void onMousePress(QPointF pos, Qt::MouseButton button);
private:
    Game* game_;
};
