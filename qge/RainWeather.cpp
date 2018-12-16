#include "RainWeather.h"

#include "Game.h"
#include "Sprite.h"
#include "Map.h"
#include "Utilities.h"
#include "Sound.h"

using namespace qge;

RainWeather::RainWeather(QPixmap rainGraphic, int numOfRains, int rainFalldownSpeed, int rainMoveAmountPerStep, int splashStepFreqMs, int numSplashPerStep, double rainInitialOpacity, double rainMaxOpacity, double rainOpacityStepSize, int rainInitialToMaxOpacityTimeMS, double splashInitialOpacity, double splashFinalOpacity, double splashOpacityStepSize, int splashInitialToFinalOpacityTimeMS):
    rainMoveTimer_(new QTimer(this)),
    rainOpacityTimer_(new QTimer(this)),
    createSplashTimer_(new QTimer(this)),
    splashOpacityTimer_(new QTimer(this)),
    rainGraphic_(rainGraphic),
    numOfRainGraphics_(numOfRains),
    fallDownSpeed_(rainFalldownSpeed),
    rainStepSize_(rainMoveAmountPerStep),
    splashStepFreq_(splashStepFreqMs),
    numSplashPerStep_(numSplashPerStep),
    rainInitialOpacity_(rainInitialOpacity),
    rainMaxOpacity_(rainMaxOpacity),
    rainOpacityStepSize_(rainOpacityStepSize),
    rainInitialToMaxOpacityTime_(rainInitialToMaxOpacityTimeMS),
    splashInitialOpacity_(splashInitialOpacity),
    splashMaxOpacity_(splashFinalOpacity),
    splashOpacityStepSize_(splashOpacityStepSize),
    splashInitialToMaxOpacityTime_(splashInitialToFinalOpacityTimeMS),
    currentSplashOpacity_(splashInitialOpacity),
    currentRainOpacity_(rainInitialOpacity),
    rainSound_(new Sound("qrc:/resources/sounds/rain.ogg",this))
{
    // connect timers
    connect(rainMoveTimer_,&QTimer::timeout,this,&RainWeather::rainMoveStep_);
    connect(rainOpacityTimer_,&QTimer::timeout,this,&RainWeather::rainOpacityStep_);
    connect(createSplashTimer_,&QTimer::timeout,this,&RainWeather::createSplashesStep_);
    connect(splashOpacityTimer_,&QTimer::timeout,this,&RainWeather::splashOpacityStep_);

    // create some rain graphics
    for (int i = 0, n = numOfRainGraphics_; i < n; i ++){
        QGraphicsPixmapItem* rain = new QGraphicsPixmapItem(rainGraphic_);
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

void RainWeather::start_()
{
    // add rain graphics to scene/set their opacities
    for (QGraphicsPixmapItem* rain:rains_){
        map_->scene()->addItem(rain);
        rain->setOpacity(rainInitialOpacity_);
    }

    // start playing sound
    rainSound_->setVolume(0);
    rainSound_->play(-1);

    startTimers_();
}

void RainWeather::stop_()
{
    // stop timers
    rainMoveTimer_->stop();
    createSplashTimer_->stop();

    // remove rain graphics from scene (splashes remove themselves after playing)
    for (QGraphicsPixmapItem* rain:rains_){
        map_->scene()->removeItem(rain);
    }

    // stop playing sound
    rainSound_->stop();
}

void RainWeather::resume_()
{
    startTimers_();
}

void RainWeather::pause_()
{
    rainMoveTimer_->stop();
    rainOpacityTimer_->stop();
    createSplashTimer_->stop();
    splashOpacityTimer_->stop();
}

/// Executed periodically to move the rain graphics down.
/// Will also move them back up (at a random x,y) when far down enough.
void RainWeather::rainMoveStep_()
{
    Game* mapsGame = map_->game();

    double screenBottomY = mapsGame->cam().bottom();
    for (QGraphicsPixmapItem* rain:rains_){
        // move down
        rain->moveBy(0,rainStepSize_);

        // move back up if too far down
        if (rain->y() > screenBottomY){
            double yPos = rand() % 700 - 700; // b/w -700 and 0
            double xPos = rand() % ((int)(mapsGame->cam().width()) + 400) - 200; // -200 -> camWidth+200
            rain->setPos(mapsGame->mapToMap(QPoint(xPos,yPos)));
        }
    }
}

/// Executed periodically to blend the rain in.
/// Once rains reach max opacity, this function will disconnect its timer.
void RainWeather::rainOpacityStep_()
{   
    if (currentRainOpacity_ < rainMaxOpacity_){
        currentRainOpacity_ += rainOpacityStepSize_;
        for (QGraphicsPixmapItem* rain:rains_)
            rain->setOpacity(currentRainOpacity_);
        rainSound_->setVolume(rainSound_->volume() + 1);
    }
    else{
        rainOpacityTimer_->disconnect(); // done raising opacity to max
    }
}

/// Executed every so often to generate splashes.
/// Will randomly generate splash graphics where the cam is currently looking at.
void RainWeather::createSplashesStep_()
{
    Game* mapsGame = map_->game();

    for (int i = 0, n = numSplashPerStep_; i < n; i++){
        Sprite* splash = new Sprite();
        splash->setOpacity(currentSplashOpacity_);
        splash->addFrames(":/resources/graphics/effects/splash",4,"splash","splash");
        double xPos = rand() % ((int)map_->game()->cam().width()); // 0 - camWidth
        double yPos = rand() % ((int)map_->game()->cam().height());  // 0 - camHeight
        QPointF pos = map_->game()->mapToMap(QPoint(xPos,yPos));
        map_->playOnce(splash,"splash",50,pos);
    }
}

/// Executed periodically to blend the splashes in.
/// Once the splashes have been blended in, this function will disconnect its timer.
void RainWeather::splashOpacityStep_()
{
    if (currentSplashOpacity_ < splashMaxOpacity_)
        currentSplashOpacity_ += splashOpacityStepSize_;
    else
        splashOpacityTimer_->disconnect();
}

void RainWeather::startTimers_()
{
    rainMoveTimer_->start(secondsToMs(frequency(rainStepSize_,fallDownSpeed_)));

    double rainOpacityRate = (rainMaxOpacity_ - rainInitialOpacity_) / rainInitialToMaxOpacityTime_; // ms
    rainOpacityTimer_->start(frequency(rainOpacityStepSize_,rainOpacityRate));

    createSplashTimer_->start(splashStepFreq_);

    double splashOpacityRate = (splashMaxOpacity_ - splashInitialOpacity_) / splashInitialToMaxOpacityTime_;
    splashOpacityTimer_->start(frequency(splashOpacityStepSize_,splashOpacityRate));
}
