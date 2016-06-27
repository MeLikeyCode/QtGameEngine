#include "RainWeather.h"
#include <QTimer>
#include "Game.h"
#include "Sprite.h"
#include "Map.h"

RainWeather::RainWeather(Game *game):
    rainTimer_(new QTimer()),
    splashTimer_(new QTimer()),
    game_(game)
{
    // connect timers
    connect(rainTimer_,&QTimer::timeout,this,&RainWeather::rainStep_);
    connect(splashTimer_,&QTimer::timeout,this,&RainWeather::splashStep_);
}

RainWeather::~RainWeather()
{
    for (QGraphicsPixmapItem* rain:rains_){
        game_->scene()->removeItem(rain);
    }
    rainTimer_->disconnect();
    splashTimer_->disconnect();
    delete rainTimer_;
    delete splashTimer_;
}

/// Starts raining.
void RainWeather::start()
{
    // create some rain graphics
    for (int i = 0, n = 25; i < n; i ++){
        QGraphicsPixmapItem* rain = new QGraphicsPixmapItem(QPixmap(":/resources/graphics/effects/rain.png"));
        rains_.push_back(rain);
        game_->scene()->addItem(rain);
    }

    rainTimer_->start(10);
    splashTimer_->start(75);
}

/// Stops raining.
void RainWeather::stop()
{
    // stop timers
    rainTimer_->stop();
    splashTimer_->stop();

    // clean graphics (splashes remove themselves)
    for (QGraphicsPixmapItem* rain:rains_){
        game_->scene()->removeItem(rain);
    }
    rains_.clear();
}

/// Executed every so often to simulate rain.
/// Will move the rain graphics down, when the reached far down enough,
/// will move them back up.
void RainWeather::rainStep_()
{
    double screenBottomY = game_->centerCamPos().y() + game_->sceneRect().height()/2;
    double screenTopY = game_->centerCamPos().y() - game_->sceneRect().height()/2;
    for (QGraphicsPixmapItem* rain:rains_){
        // move down
        rain->moveBy(0,100);

        // move back up if too far down
        if (rain->y() > screenBottomY){
            double yPos = rand() % 700 - 700; // b/w -700 and 0
            double xPos = rand() % ((int)(game_->cam().width()) + 400) - 200; // -200 -> camWidth+200
            rain->setPos(game_->mapToMap(QPoint(xPos,yPos)));
        }
    }
}

/// Executed every so often to simulate the splashes of the rain.
/// Will randomly generate splash graphics where the cam is currently looking at.
void RainWeather::splashStep_()
{
    for (int i = 0, n = 7; i < n; i++){
        Sprite* splash = new Sprite();
        splash->addFrames(":/resources/graphics/effects/splash",4,"splash");
        double xPos = rand() % ((int)game_->cam().width()); // 0 - camWidth
        double yPos = rand() % ((int)game_->cam().height());  // 0 - camHeight
        QPointF pos = game_->mapToMap(QPoint(xPos,yPos));
        game_->map()->playOnce(splash,"splash",50,pos);
    }
}
