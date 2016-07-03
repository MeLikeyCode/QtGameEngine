#include "SnowWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <cassert>
#include "Game.h"

SnowWeather::SnowWeather():
    snowTimer_(new QTimer()),
    started_(false),
    initial_(true)
{
    // connect timer
    connect(snowTimer_,&QTimer::timeout,this,&SnowWeather::snowStep_);
}

SnowWeather::~SnowWeather()
{
    // delete all snow graphics from the scene of the Map (if it has a Map)
    if (map_){
        for (QGraphicsPixmapItem* snow:snows_){
            map_->scene()->removeItem(snow);
        }
    }
}

/// Starts snowing. Does nothing if the snow has already started.
void SnowWeather::start()
{
    // make sure has a map
    assert(map_ != nullptr);

    // do nothing already started
    if (started_){
        return;
    }

    // create some snow graphics at the top of screen
    for (int i = 0, n = 30; i < n; i ++){
        QGraphicsPixmapItem* snow = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/snow/snow0.png"));
        snows_.push_back(snow);
        map_->scene()->addItem(snow);
        snow->setOpacity(0.05);
    }

    snowTimer_->start(100);

    started_ = true;
}

/// Stops snowing. Does nothing if the snow has already stopped.
void SnowWeather::stop()
{
    snowTimer_->stop();

    // clean graphics
    for (QGraphicsPixmapItem* snow:snows_){
        map_->scene()->removeItem(snow);
    }
    snows_.clear();
}

void SnowWeather::snowStep_()
{
    assert(map_ != nullptr);    // make sure we have a Map

    // do nothing if this Map isn't *currently* in a Game
    Game* mapsGame = map_->game();
    if (mapsGame == nullptr){
        return;
    }
    if (mapsGame->currentMap() != map_){
        return;
    }

    if (initial_){
        // position snows at top of camera
        for (QGraphicsPixmapItem* snow:snows_){
            double randYOffset = rand() % ((int)(mapsGame->cam().height()*3)); // 0 - 2*height of cam
            double randXOffset = rand() % (int)(mapsGame->cam().width() + 300) - 300; // -300 -> right side of cam
            snow->setPos(mapsGame->cam().topLeft());
            snow->setY(snow->y() - snow->boundingRect().height() - randYOffset);
            snow->setX(snow->x() + randXOffset);
        }
        initial_ = false;
    }

    double screenBottomY = map_->game()->centerCamPos().y() + map_->game()->sceneRect().height()/2;
    for (QGraphicsPixmapItem* snow:snows_){
        // move down
        snow->moveBy(0,20);
        snow->setOpacity(snow->opacity() + 0.005);

        // move back up if too far down
        if (snow->y() > screenBottomY){
            double randYOffset = rand() % ((int)(mapsGame->cam().height()*3)); // 0 - 2*height of cam
            double randXOffset = rand() % (int)(mapsGame->cam().width() + 300) - 300; // -300 -> right side of cam
            snow->setPos(mapsGame->cam().topLeft());
            snow->setY(snow->y() - snow->boundingRect().height() - randYOffset);
            snow->setX(snow->x() + randXOffset);
        }
    }
}
