#include "FogWeather.h"
#include <QTimer>
#include "Map.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "Game.h"
#include <cassert>
#include "Utilities.h"
#include <QRectF>

FogWeather::FogWeather() :
    opacityTimer_(new QTimer(this)),
    moveTimer_(new QTimer(this)),
    initialOpacity_(0.05),
    maxOpacity_(0.5),
    opacityFadeTime_(2000),
    opacityStepSize_(0.005),
    fogSpeed_(300),
    fogStepSize_(5),
    fogDirection_(QVector2D(0,1)) // down
{
    // connect timers
    connect(opacityTimer_,&QTimer::timeout,this,&FogWeather::opacityStep_);
    connect(moveTimer_,&QTimer::timeout,this,&FogWeather::moveStep_);

    currentOpacity_ = initialOpacity_;
}

FogWeather::~FogWeather()
{
    // remove/delete all fog squares
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        map_->scene()->removeItem(fogSquare);
        delete fogSquare;
    }

}

void FogWeather::start_()
{
    Game* mapsGame = map_->game();

    // create fog squares
    int numFogSquresNeededX = mapsGame->cam().width() / 500 + 3; // TODO: replace 500 w/ width of pixmap (+2 needed!)
    int numFogSquresNeededY = mapsGame->cam().height() / 500 + 3; // TODO: same as above
    for (int i = 0, n = numFogSquresNeededX; i < n; i++){
        for (int j = 0, p = numFogSquresNeededY; j < p; j++){
            QPixmap fogPm(":/resources/graphics/effects/fog.png");
            fogPm = fogPm.scaled(500,500); // TODO: replace 500 with size of pixmap

            QGraphicsPixmapItem* fogSquare = new QGraphicsPixmapItem(fogPm);
            fogSquare->setZValue(Map::Z_VALUES::WEATHER_Z_VALUE);
            fogSquare->setOpacity(initialOpacity_);
            map_->scene()->addItem(fogSquare);

            fogSquares_.insert(fogSquare);
        }
    }

    // set fog squares positions
    double fogBoundryWidth = numFogSquresNeededX * 500; // TODO: replace 500 with width of pixmap
    double fogBoundryHeight = numFogSquresNeededY * 500; // TODO: same as above
    fogBoundry_ = QRectF(0,0,fogBoundryWidth,fogBoundryHeight);
    fogBoundry_.moveCenter(mapsGame->centerCamPos());

    int cellx = 0;
    int celly = 0;
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        double actualx = cellx * 500 + fogBoundry_.topLeft().x(); // TODO: replace with width (500)
        double actualy = celly * 500 + fogBoundry_.topLeft().y(); // TODO: replace with height (500)
        fogSquare->setPos(actualx,actualy);
        cellx++;
        if (cellx >= numFogSquresNeededX){
            cellx = 0;
            celly++;
        }
    }

    // start timer to move fog squres
    startTimers_();
}

void FogWeather::resume_()
{
    startTimers_();
}

void FogWeather::pause_()
{
    opacityTimer_->stop();
    moveTimer_->stop();
}

void FogWeather::stop_()
{
    opacityTimer_->stop();
    moveTimer_->stop();

    // remove all fog squares
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        map_->scene()->removeItem(fogSquare);
    }
}

/// Executed periodically to blend the fog in (slowly increase its opacity).
void FogWeather::opacityStep_()
{
    // increase opacities
    if (currentOpacity_ < maxOpacity_){
        currentOpacity_ += opacityStepSize_;
        for (QGraphicsPixmapItem* fogSquare:fogSquares_)
            fogSquare->setOpacity(currentOpacity_);
    }

    // if opacities increased enough, disconnect
    if (currentOpacity_ >= maxOpacity_)
        opacityTimer_->disconnect();
}

/// Executed periodically to move the fog graphics.
void FogWeather::moveStep_()
{
    Game* mapsGame = map_->game();

    // move all fog squares down
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        fogSquare->moveBy(0,fogStepSize_);
    }

    fogBoundry_.moveCenter(mapsGame->centerCamPos());
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        // if the fogsquare is too far down, move it back up
        if (fogSquare->y() > fogBoundry_.bottom()){
            double offset = fogSquare->y() - fogBoundry_.bottom();
            fogSquare->setY(fogBoundry_.top() + offset);
        }
        // if the fogsquare is too far up, move it back down
        if (fogSquare->y() + 500 < fogBoundry_.top()){
            double offset = fogBoundry_.top() - (fogSquare->y() + 500);
            fogSquare->setY(fogBoundry_.bottom() - 500 - offset);
        }
        // if the forgsquare is too far left, move it to right
        if (fogSquare->x() + 500 < fogBoundry_.left()){
            double offset = fogBoundry_.left() - (fogSquare->x() + 500);
            fogSquare->setX(fogBoundry_.right() - 500 - offset);
        }
        // if the fogsquare is too far right, move it to the left
        if (fogSquare->x() > fogBoundry_.right()){
            double offset = fogSquare->x() - fogBoundry_.right();
            fogSquare->setX(fogBoundry_.left() + offset);
        }
    }
}

void FogWeather::startTimers_()
{
    double opacityRate = (maxOpacity_ - initialOpacity_) / opacityFadeTime_; // units per ms

    opacityTimer_->start(frequency(opacityStepSize_, opacityRate));
    moveTimer_->start(secondsToMs(frequency(fogStepSize_,fogSpeed_)));
}
