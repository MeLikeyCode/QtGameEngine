#include "RainWeather.h"
#include <QTimer>
#include "Game.h"
#include "Sprite.h"
#include "Map.h"
#include <cassert>

RainWeather::RainWeather(int numOfRainGraphics, int rainStepFreqMs, int rainMoveAmountPerStep, int splashStepFreq, int numSplashPerStep):
    rainTimer_(new QTimer(this)),
    splashTimer_(new QTimer(this)),
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

    // create some rain graphics
    for (int i = 0, n = numOfRainGraphics_; i < n; i ++){
        QGraphicsPixmapItem* rain = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/rain.png"));
        rains_.push_back(rain);
    }

}

RainWeather::~RainWeather()
{
    // clean up rain graphics
    if (map_){
        for (QGraphicsPixmapItem* rain:rains_){
            map_->scene()->removeItem(rain);    // remove from scene
            delete rain;                        // delete
        }
    }
}

/// Starts raining. Does nothing if the rain has already started.
void RainWeather::start()
{
    // make sure has a Map
    assert(map_ != nullptr);

    // do nothing if already started
    if (started_){
        return;
    }

    // add rain graphics to scene/set their opacities
    for (QGraphicsPixmapItem* rain:rains_){
        map_->scene()->addItem(rain);
        rain->setOpacity(0.08);
        rain->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
    }

    rainTimer_->start(rainStepFreqMs_);
    splashTimer_->start(splashStepFreq_);

    started_ = true;
    currentSplashOpacity_ = 0.08;
}

/// Stops raining. Does nothing if the rain is already stopped.
void RainWeather::stop()
{
    // stop timers
    rainTimer_->stop();
    splashTimer_->stop();

    // remove rain graphics from scene (splashes remove themselves after playing)
    for (QGraphicsPixmapItem* rain:rains_){
        map_->scene()->removeItem(rain);
    }

    started_ = false;
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

    double screenBottomY = mapsGame->cam().bottom();
    for (QGraphicsPixmapItem* rain:rains_){
        // move down
        rain->moveBy(0,rainMoveAmountPerStep_);

        // increase opacity
        rain->setOpacity(rain->opacity() + 0.0005);

        // move back up if too far down
        if (rain->y() > screenBottomY){
            double yPos = rand() % 700 - 700; // b/w -700 and 0
            double xPos = rand() % ((int)(mapsGame->cam().width()) + 400) - 200; // -200 -> camWidth+200
            rain->setPos(mapsGame->mapToMap(QPoint(xPos,yPos)));
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

    if (currentSplashOpacity_ < 0.5){
        currentSplashOpacity_ += 0.002;
    }
    else {
        int r = rand() % 5;
        if (numSplashPerStep_ < 60 && r == 1)
        numSplashPerStep_++;
    }

    for (int i = 0, n = numSplashPerStep_; i < n; i++){
        Sprite* splash = new Sprite();
        splash->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
        splash->setOpacity(currentSplashOpacity_);
        splash->addFrames(":/resources/graphics/effects/splash",4,"splash");
        double xPos = rand() % ((int)map_->game()->cam().width()); // 0 - camWidth
        double yPos = rand() % ((int)map_->game()->cam().height());  // 0 - camHeight
        QPointF pos = map_->game()->mapToMap(QPoint(xPos,yPos));
        map_->playOnce(splash,"splash",50,pos);
    }
}
