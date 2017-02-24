#include "FogWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "Game.h"
#include <cassert>

FogWeather::FogWeather() :
    opacityTimer_(new QTimer(this)),
    fog1_(nullptr),
    fog2_(nullptr),
    initial_(true),
    initialOpacity_(0.05),
    maxOpacity_(0.5)
{
    // connect timers
    connect(opacityTimer_,&QTimer::timeout,this,&FogWeather::opacityStep_);
    connect(moveTimer_&QTimer::timeout,this,&FogWeather::moveStep_);
}

FogWeather::~FogWeather()
{
    // delete fog graphics (if they were created)
    if (fog1_ != nullptr){
        map_->scene()->removeItem(fog1_);
        delete fog1_;
    }
    if (fog2_ != nullptr){
        map_->scene()->removeItem(fog2_);
        delete fog2_;
    }
}

void FogWeather::start_()
{
    opacityTimer_->start(10); // TODO replace 10 ms with needed time to get needed speed
    moveTimer_->start(10);
}

void FogWeather::stop()
{
    opacityTimer_->stop();

    // remove the fog graphics (if they have been created)
    if (fog1_ != nullptr)
        map_->scene()->removeItem(fog1_);
    if (fog2_ != nullptr)
        map_->scene()->removeItem(fog2_);

    initial_ = true;
}

/// Executed periodically to blend the fog in (slowly increase its opacity).
void FogWeather::opacityStep_()
{
    assert(map_ != nullptr);

    // do nothing if this Map isn't *currently* in a Game
    Game* mapsGame = map_->game();
    if (mapsGame->currentMap() != map_)
        return;

    createGraphicsIfNeeded_();

    // increase opacities
    if (fog1_->opacity() < maxOpacity_){
        fog1_->setOpacity(fog1_->opacity() + 0.005);
        fog2_->setOpacity(fog2_->opacity() + 0.005);
    }
    // if opacity increased fully, stop calling this function
    else{
        opacityTimer_->disconnect();
    }
}

/// Executed periodically to move the fog graphics down.
void FogWeather::moveStep_()
{
    assert(map_ != nullptr);

    // do nothing if this Map isn't *currently* in a Game
    Game* mapsGame = map_->game();
    if (mapsGame->currentMap() != map_)
        return;

    createGraphicsIfNeeded_();

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

/// Creates the fog graphics if they have not already been created.
void FogWeather::createGraphicsIfNeeded_()
{
    if (initial_){
        // create fog1 and fog2 images
        QPixmap pm(":/resources/graphics/effects/fog.png");
        pm = pm.scaled(mapsGame->cam().width(),mapsGame->cam().height() + 300);
        fog1_ = new QGraphicsPixmapItem(pm);
        fog2_ = new QGraphicsPixmapItem(pm);
        fog1_->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
        fog2_->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
        fog1_->setOpacity(initialOpacity_);
        fog2_->setOpacity(initialOpacity_);
        map_->scene()->addItem(fog1_);
        map_->scene()->addItem(fog2_);

        // set up fog1 and 2
        QPointF camPos = mapsGame->cam().topLeft();
        fog1_->setPos(camPos);
        fog2_->setPos(camPos);
        fog2_->setY(fog2_->y() - fog2_->boundingRect().height());

        initial_ = false;
    }
}
