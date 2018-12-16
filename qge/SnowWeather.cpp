#include "SnowWeather.h"

#include "Map.h"
#include "Game.h"

using namespace qge;

SnowWeather::SnowWeather():
    globularSnowTimer_(new QTimer(this)),
    linearSnowTimer_(new QTimer(this)),
    snow1_(nullptr),
    snow2_(nullptr),
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

void SnowWeather::start_()
{
    Game* mapsGame = map_->game();

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

    // create linear snow (snow1 and snow2)
    QPixmap pm(":/resources/graphics/effects/snow/snow1.png");
    pm = pm.scaled(mapsGame->cam().width(),pm.height());
    snow1_ = new QGraphicsPixmapItem(pm);
    snow2_ = new QGraphicsPixmapItem(pm);
    snow1_->setOpacity(0.05);
    snow2_->setOpacity(0.05);
    map_->scene()->addItem(snow1_);
    map_->scene()->addItem(snow2_);

    // position snow1 and 2
    QPointF camPos = mapsGame->cam().topLeft();
    snow1_->setPos(camPos);
    snow2_->setPos(camPos);
    snow2_->setY(snow2_->y() - snow2_->boundingRect().height());

    // start timers
    startTimers_();
}

void SnowWeather::stop_()
{
    globularSnowTimer_->stop();

    // clean graphics
    for (QGraphicsPixmapItem* snow:globularSnows_){
        map_->scene()->removeItem(snow);
    }
    globularSnows_.clear();

    // remove snow graphics (if they have been created)
    if (snow1_ != nullptr)
        map_->scene()->removeItem(snow1_);
    if (snow2_ != nullptr)
        map_->scene()->removeItem(snow2_);

    globularSnowTimer_->stop();
    linearSnowTimer_->stop();
}

void SnowWeather::resume_()
{
    startTimers_();
}

void SnowWeather::pause_()
{
    globularSnowTimer_->stop();
    linearSnowTimer_->stop();
}

void SnowWeather::snowStepGlobular_()
{
    Game* mapsGame = map_->game();

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
    Game* mapsGame = map_->game();

    // increase opacities
    if (snow1_->opacity() < 1){
        snow1_->setOpacity(snow1_->opacity() + 0.005);
        snow2_->setOpacity(snow2_->opacity() + 0.005);
    }

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

void SnowWeather::startTimers_()
{
    globularSnowTimer_->start(100);
    linearSnowTimer_->start(100);
}
