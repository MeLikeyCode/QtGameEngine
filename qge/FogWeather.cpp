#include "FogWeather.h"

#include "Utilities.h"
#include "Map.h"
#include "Game.h"

using namespace qge;

FogWeather::FogWeather(QPixmap tileableFogGraphic, int tileWidth, int tileHeight, double initialOpacity, double finalOpacity, double opacityFadeTimeMs, double opacityStepSize) :
    opacityTimer_(new QTimer(this)),
    moveTimer_(new QTimer(this)),
    initialOpacity_(initialOpacity),
    maxOpacity_(finalOpacity),
    opacityFadeTime_(opacityFadeTimeMs),
    opacityStepSize_(opacityStepSize),
    fogSpeed_(50),
    fogStepSize_(5),
    fogDirection_(QVector2D(0,1)), // down
    fogPicture_(tileableFogGraphic),
    fogPictureHeight_(tileHeight),
    fogPictureWidth_(tileWidth)
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
    int numFogSquresNeededX = mapsGame->cam().width() / fogPictureWidth_ + 3;
    int numFogSquresNeededY = mapsGame->cam().height() / fogPictureHeight_ + 3;
    for (int i = 0, n = numFogSquresNeededX; i < n; i++){
        for (int j = 0, p = numFogSquresNeededY; j < p; j++){
            fogPicture_ = fogPicture_.scaled(fogPictureWidth_,fogPictureHeight_);

            QGraphicsPixmapItem* fogSquare = new QGraphicsPixmapItem(fogPicture_);
            fogSquare->setOpacity(initialOpacity_);
            map_->scene()->addItem(fogSquare);

            fogSquares_.insert(fogSquare);
        }
    }

    // set fog squares positions
    double fogBoundryWidth = numFogSquresNeededX * fogPictureWidth_;
    double fogBoundryHeight = numFogSquresNeededY * fogPictureHeight_;
    fogBoundry_ = QRectF(0,0,fogBoundryWidth,fogBoundryHeight);
    fogBoundry_.moveCenter(mapsGame->centerCamPos());

    int cellx = 0;
    int celly = 0;
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        double actualx = cellx * fogPictureWidth_ + fogBoundry_.topLeft().x();
        double actualy = celly * fogPictureHeight_ + fogBoundry_.topLeft().y();
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

/// Sets how fast the fog should travel.
void FogWeather::setFogSpeed(double pixelsPerSecond)
{
    fogSpeed_ = pixelsPerSecond;
    pause_();
    resume_();
}

/// Sets the granularity of movement of the fog.
/// The lower, the smoother the movement but the more expensive.
void FogWeather::setFogStepSize(double numPixels)
{
    fogStepSize_ = numPixels;
    pause_();
    resume_();
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
    else
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

    // set fog boundry based on location of cam
    fogBoundry_.moveCenter(mapsGame->centerCamPos());
    for (QGraphicsPixmapItem* fogSquare:fogSquares_){
        double fogSquareLeft = fogSquare->x();
        double fogSquareRight = fogSquare->x() + fogPictureWidth_;
        double fogSquareTop = fogSquare->y();
        double fogSquareBottom = fogSquare->y() + fogPictureHeight_;

        // if the fog square is too far down, move it back up
        if (fogSquareTop > fogBoundry_.bottom()){
            double offset = fogSquareTop - fogBoundry_.bottom();
            fogSquare->setY(fogBoundry_.top() + offset);
        }
        // if the fog square is too far up, move it back down
        if (fogSquareBottom < fogBoundry_.top()){
            double offset = fogBoundry_.top() - fogSquareBottom;
            fogSquare->setY(fogBoundry_.bottom() - fogPictureHeight_ - offset);
        }
        // if the fog square is too far left, move it to right
        if (fogSquareRight < fogBoundry_.left()){
            double offset = fogBoundry_.left() - fogSquareRight;
            fogSquare->setX(fogBoundry_.right() - fogPictureWidth_ - offset);
        }
        // if the fog square is too far right, move it to the left
        if (fogSquareLeft > fogBoundry_.right()){
            double offset = fogSquareLeft - fogBoundry_.right();
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
