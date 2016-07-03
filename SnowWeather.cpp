#include "SnowWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <cassert>
#include "Game.h"
#include <QDebug>

SnowWeather::SnowWeather():
    snowTimer_(new QTimer()),
    snowTimerDuel_(new QTimer()),
    started_(false),
    initial_(true),
    initialDuel_(true)
{
    // connect timer
    connect(snowTimer_,&QTimer::timeout,this,&SnowWeather::snowStepGlobular_);
    connect(snowTimerDuel_,&QTimer::timeout,this,&SnowWeather::snowStepDuel_);
}

SnowWeather::~SnowWeather()
{
    // delete all snow graphics from the scene of the Map (if it has a Map)
    if (map_){
        for (QGraphicsPixmapItem* snow:snows_){
            map_->scene()->removeItem(snow);
        }
    }

    map_->scene()->removeItem(snow1_);
    map_->scene()->removeItem(snow2_);
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

//    // create some snow graphics at the top of screen
//    for (int i = 0, n = 15; i < n; i ++){
//        QGraphicsPixmapItem* snow = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/snow/snow0.png"));
//        snows_.push_back(snow);
//        map_->scene()->addItem(snow);
//        snow->setOpacity(0.05);
//    }

    // create snow1 and snow2 images
    snow1_ = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/snow/snow1.png"));
    snow2_ = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/snow/snow1.png"));
    snow1_->setOpacity(0.05);
    snow2_->setOpacity(0.05);
    map_->scene()->addItem(snow1_);
    map_->scene()->addItem(snow2_);

    //snowTimer_->start(100);
    snowTimerDuel_->start(100);

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

    map_->scene()->removeItem(snow1_);
    map_->scene()->removeItem(snow2_);
}

void SnowWeather::snowStepGlobular_()
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
            double randYOffset = rand() % ((int)(mapsGame->cam().height()*2)); // 0 - 2*height of cam
            double randXOffset = rand() % (int)(mapsGame->cam().width() + 300) - 300; // -300 -> right side of cam
            snow->setPos(mapsGame->cam().topLeft());
            snow->setY(snow->y() - snow->boundingRect().height() - randYOffset);
            snow->setX(snow->x() + randXOffset);
        }
        initial_ = false;
    }

    double screenBottomY = map_->game()->centerCamPos().y() + map_->game()->sceneRect().height()/2;
    for (QGraphicsPixmapItem* snow:snows_){
        // move down and left
        snow->moveBy(0,20);
        snow->setOpacity(snow->opacity() + 0.005);

        // move back up if too far down
        if (snow->y() > screenBottomY){
            double randYOffset = rand() % ((int)(mapsGame->cam().height()*2)); // 0 - 2*height of cam
            double randXOffset = rand() % (int)(mapsGame->cam().width() + 300) - 300; // -300 -> right side of cam
            snow->setPos(mapsGame->cam().topLeft());
            snow->setY(snow->y() - snow->boundingRect().height() - randYOffset);
            snow->setX(snow->x() + randXOffset);
        }
    }
}

void SnowWeather::snowStepDuel_()
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

    if (initialDuel_){
        // set up snow1 and 2
        QPointF camPos = mapsGame->cam().topLeft();
        snow1_->setPos(camPos);
        snow2_->setPos(camPos);
        snow1_->setY(snow1_->y() - snow1_->boundingRect().height());
        snow2_->setY(snow2_->y() - 2 * snow2_->boundingRect().height());
        initialDuel_ = false;
    }

    // increase opacities
    snow1_->setOpacity(snow1_->opacity() + 0.005);
    snow2_->setOpacity(snow2_->opacity() + 0.005);

    // make sure both images are in the screen x wise
    snow1_->setX(mapsGame->cam().topLeft().x());
    snow2_->setX(mapsGame->cam().topLeft().x());

    // move down both images
    snow1_->setY(snow1_->y() + 20);
    snow2_->setY(snow2_->y() + 20);

    // if snow1 is at bottom, move it ontop of snow2
    double bottomY = mapsGame->cam().bottom();
    if (snow1_->y() > bottomY){
        snow1_->setY(snow2_->y() - snow1_->boundingRect().height());
    }

    // if snow2 is at bottom, move it ontop of snow1
    if (snow2_->y() > bottomY){
        snow2_->setY(snow1_->y() - snow2_->boundingRect().height());
    }
}
