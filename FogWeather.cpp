#include "FogWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "Game.h"
#include <cassert>

FogWeather::FogWeather() :
    fogTimer_(new QTimer(this)),
    started_(false),
    initial_(true),
    maxFogOpacity_(0.5)
{
    // connect timer
    connect(fogTimer_,&QTimer::timeout,this,&FogWeather::fogStep_);
}

FogWeather::~FogWeather()
{
    map_->scene()->removeItem(fog1_);
    map_->scene()->removeItem(fog2_);

    delete fog1_;
    delete fog2_;
}

void FogWeather::start()
{
    // make sure has a map
    assert(map_ != nullptr);

    // do nothing already started
    if (started_){
        return;
    }

    fogTimer_->start(100);

    started_ = true;
}

void FogWeather::stop()
{
    fogTimer_->stop();

    map_->scene()->removeItem(fog1_);
    map_->scene()->removeItem(fog2_);

    initial_ = true;
    started_ = false;
}

void FogWeather::fogStep_()
{
    assert(map_ != nullptr);

    // do nothing if this Map isn't *currently* in a Game
    Game* mapsGame = map_->game();
    if (mapsGame->currentMap() != map_){
        return;
    }

    if (initial_){
        // create fog1 and fog2 images
        QPixmap pm(":/resources/graphics/effects/fog.png");
        pm = pm.scaled(mapsGame->cam().width(),mapsGame->cam().height() + 300);
        fog1_ = new QGraphicsPixmapItem(pm);
        fog2_ = new QGraphicsPixmapItem(pm);
        fog1_->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
        fog2_->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
        fog1_->setOpacity(0.05);
        fog2_->setOpacity(0.05);
        map_->scene()->addItem(fog1_);
        map_->scene()->addItem(fog2_);

        // set up fog1 and 2
        QPointF camPos = mapsGame->cam().topLeft();
        fog1_->setPos(camPos);
        fog2_->setPos(camPos);
        fog2_->setY(fog2_->y() - fog2_->boundingRect().height());
        initial_ = false;
    }

    // increase opacities
    if (fog1_->opacity() < maxFogOpacity_){
        fog1_->setOpacity(fog1_->opacity() + 0.005);
        fog2_->setOpacity(fog2_->opacity() + 0.005);
    }

    // make sure both images are in the screen x wise
    fog1_->setX(mapsGame->cam().topLeft().x());
    fog2_->setX(mapsGame->cam().topLeft().x());

    // move down both images
    fog1_->setY(fog1_->y() + 5);
    fog2_->setY(fog2_->y() + 5);

    // if fog1 is at bottom, move it ontop of fog2
    double bottomY = mapsGame->cam().bottom();
    if (fog1_->y() > bottomY){
        fog1_->setY(fog2_->y() - fog1_->boundingRect().height());
    }

    // if fog2 is at bottom, move it ontop of fog1
    if (fog2_->y() > bottomY){
        fog2_->setY(fog1_->y() - fog2_->boundingRect().height());
    }
}
