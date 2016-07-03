#include "SnowWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <cassert>
#include "Game.h"
#include <QDebug>

SnowWeather::SnowWeather():
    globularSnowTimer_(new QTimer(this)),
    linearSnowTimer_(new QTimer(this)),
    started_(false),
    initialGlobular_(true),
    initialLinear_(true)
{
    // connect timer
    connect(globularSnowTimer_,&QTimer::timeout,this,&SnowWeather::snowStepGlobular_);
    connect(linearSnowTimer_,&QTimer::timeout,this,&SnowWeather::snowStepLinear_);
}

SnowWeather::~SnowWeather()
{
    // delete all snow graphics from the scene of the Map (if it has a Map)
    if (map_){
        for (QGraphicsPixmapItem* snow:globularSnows_){
            map_->scene()->removeItem(snow);
        }
    }

    map_->scene()->removeItem(snow1_);
    map_->scene()->removeItem(snow2_);

    delete snow1_;
    delete snow2_;
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

    globularSnowTimer_->start(100);
    linearSnowTimer_->start(100);

    started_ = true;
}

/// Stops snowing. Does nothing if the snow has already stopped.
void SnowWeather::stop()
{
    globularSnowTimer_->stop();

    // clean graphics
    for (QGraphicsPixmapItem* snow:globularSnows_){
        map_->scene()->removeItem(snow);
    }
    globularSnows_.clear();

    map_->scene()->removeItem(snow1_);
    map_->scene()->removeItem(snow2_);

    globularSnowTimer_->stop();
    linearSnowTimer_->stop();

    initialGlobular_ = true;
    initialLinear_ = true;
    started_ = false;
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

    if (initialGlobular_){
        // create some globular snow
        for (int i = 0, n = 10; i < n; i ++){
            QGraphicsPixmapItem* snow = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/snow/snow0.png"));
            globularSnows_.push_back(snow);
            map_->scene()->addItem(snow);
            snow->setOpacity(0.05);
        }

        // position snows at top of camera
        for (QGraphicsPixmapItem* snow:globularSnows_){
            double randYOffset = rand() % ((int)(mapsGame->cam().height()*2)); // 0 - 2*height of cam
            double randXOffset = rand() % (int)(mapsGame->cam().width() + 300) - 300; // -300 -> right side of cam
            snow->setPos(mapsGame->cam().topLeft());
            snow->setY(snow->y() - snow->boundingRect().height() - randYOffset);
            snow->setX(snow->x() + randXOffset);
        }
        initialGlobular_ = false;
    }

    double screenBottomY = map_->game()->centerCamPos().y() + map_->game()->sceneRect().height()/2;
    for (QGraphicsPixmapItem* snow:globularSnows_){
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

void SnowWeather::snowStepLinear_()
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

    if (initialLinear_){
        // create snow1 and snow2 images
        QPixmap pm(":/resources/graphics/effects/snow/snow1.png");
        pm = pm.scaled(mapsGame->cam().width(),pm.height());
        snow1_ = new QGraphicsPixmapItem(pm);
        snow2_ = new QGraphicsPixmapItem(pm);
        snow1_->setOpacity(0.05);
        snow2_->setOpacity(0.05);
        map_->scene()->addItem(snow1_);
        map_->scene()->addItem(snow2_);

        // set up snow1 and 2
        QPointF camPos = mapsGame->cam().topLeft();
        snow1_->setPos(camPos);
        snow2_->setPos(camPos);
        snow1_->setY(snow1_->y() - snow1_->boundingRect().height());
        snow2_->setY(snow2_->y() - 2 * snow2_->boundingRect().height());
        initialLinear_ = false;
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
