#include "RainWeather.h"
#include <QTimer>
#include "Game.h"
#include "Sprite.h"
#include "Map.h"
#include <cassert>

RainWeather::RainWeather(int numOfRainGraphics, int rainStepFreqMs, int rainMoveAmountPerStep, int splashStepFreq, int numSplashPerStep):
    rainTimer_(new QTimer()),
    splashTimer_(new QTimer()),
    numOfRainGraphics_(numOfRainGraphics),
    rainStepFreqMs_(rainStepFreqMs),
    rainMoveAmountPerStep_(rainMoveAmountPerStep),
    splashStepFreq_(splashStepFreq),
    numSplashPerStep_(numSplashPerStep),
    started_(false)
{
    // connect timers
    connect(rainTimer_,&QTimer::timeout,this,&RainWeather::rainStep_);
    connect(splashTimer_,&QTimer::timeout,this,&RainWeather::splashStep_);
}

RainWeather::~RainWeather()
{
    // delete all rain graphics from the scene of the map (if it has a map)
    if (map_){
        for (QGraphicsPixmapItem* rain:rains_){
            map_->scene()->removeItem(rain);
        }
    }
    rainTimer_->disconnect();
    splashTimer_->disconnect();
    delete rainTimer_;
    delete splashTimer_;
}

/// Starts raining. Does nothing if the rain has already started.
void RainWeather::start()
{
    // make sure has a Map
    assert(map_ != nullptr);

    // do nothing already started
    if (started_){
        return;
    }

    // create some rain graphics
    for (int i = 0, n = numOfRainGraphics_; i < n; i ++){
        QGraphicsPixmapItem* rain = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/rain.png"));
        rains_.push_back(rain);
        map_->scene()->addItem(rain);
    }

    rainTimer_->start(rainStepFreqMs_);
    splashTimer_->start(splashStepFreq_);

    started_ = true;
}

/// Stops raining. Does nothing if the rain is already stopped.
void RainWeather::stop()
{
    // stop timers
    rainTimer_->stop();
    splashTimer_->stop();

    // clean graphics (splashes remove themselves)
    for (QGraphicsPixmapItem* rain:rains_){
        map_->scene()->removeItem(rain);
    }
    rains_.clear();
}

/// Executed every so often to simulate rain.
/// Will move the rain graphics down, when the reached far down enough,
/// will move them back up.
void RainWeather::rainStep_()
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

    double screenBottomY = map_->game()->centerCamPos().y() + map_->game()->sceneRect().height()/2;
    for (QGraphicsPixmapItem* rain:rains_){
        // move down
        rain->moveBy(0,rainMoveAmountPerStep_);

        // move back up if too far down
        if (rain->y() > screenBottomY){
            double yPos = rand() % 700 - 700; // b/w -700 and 0
            double xPos = rand() % ((int)(map_->game()->cam().width()) + 400) - 200; // -200 -> camWidth+200
            rain->setPos(map_->game()->mapToMap(QPoint(xPos,yPos)));
        }
    }
}

/// Executed every so often to simulate the splashes of the rain.
/// Will randomly generate splash graphics where the cam is currently looking at.
void RainWeather::splashStep_()
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

    for (int i = 0, n = numSplashPerStep_; i < n; i++){
        Sprite* splash = new Sprite();
        splash->addFrames(":/resources/graphics/effects/splash",4,"splash");
        double xPos = rand() % ((int)map_->game()->cam().width()); // 0 - camWidth
        double yPos = rand() % ((int)map_->game()->cam().height());  // 0 - camHeight
        QPointF pos = map_->game()->mapToMap(QPoint(xPos,yPos));
        map_->playOnce(splash,"splash",50,pos);
    }
}
