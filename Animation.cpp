#include "Animation.h"

#include <QTimer>
#include <stdexcept>

#include "SpriteSheet.h"
#include "Utilities.h"
#include "Node.h"

/// Constructs an Animation with no frames. Use addFrame() or addFrames() to add frames.
Animation::Animation(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();
}

/// Constructs an Animation with just 1 frame. Use addFrame() or addFrames() to add
/// additional frames.
Animation::Animation(const QPixmap &pixmap, QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();

    addFrame(pixmap);
    currentPixmap_->setPixmap(pixmap);
}

/// Constructs an Animation with the specified sequence of frames from the
/// specified SpriteSheet.
Animation::Animation(const SpriteSheet &spriteSheet, Node from, Node to, QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();

    addFrames(spriteSheet,from,to);
    currentPixmap_->setPixmap(pixmaps_[0]);
}

/// Adds a frame to the Animation.
void Animation::addFrame(const QPixmap &pixmap)
{
    pixmaps_.push_back(pixmap);
}

/// Adds the specified sequence of frames from the specified SpriteSheet to the Animation.
void Animation::addFrames(const SpriteSheet &fromSpriteSheet, const Node &startCell, const Node &endCell)
{
    std::vector<QPixmap> frames = fromSpriteSheet.tilesAt(startCell,endCell);
    for (QPixmap& frame:frames){
        addFrame(frame);
    }
}

/// Sets the currently displayed frame of the Animation.
/// If the animation is currently playing, it will jump to this frame and continue playing.
/// If the animation is not currently playing, whenever it is asked to play, it will
/// start from this frame.
void Animation::setCurrentFrame(int frameNumber)
{
    // enforce pre conditions
    if (frameNumber < 0)
        throw std::out_of_range("frameNumber cannot be negative");
    if (frameNumber >= pixmaps_.size())
        throw std::out_of_range("frameNumber must be less than the total number of frames");

    currentFrame_ = frameNumber;
    currentPixmap_->setPixmap(pixmaps_[currentFrame_]); // set current pixmap to be this frame
}

/// Plays the Animation the specified number of times at the specified frames per second.
/// Pass -1 for numTimesToPlay to play the animation an infinite number of times (i.e. loop infinitely).
/// Does nothing if the animation is already playing.
/// If you want to change the number of times to play it or to change the frames per second, first pause
/// the animation, then call this function again.
void Animation::play(int numTimesToPlay, double framesPerSecondToPlayAt)
{
    if (isPlaying_)
        return;

    timesToPlay_ = numTimesToPlay;
    framesPerSecond_ = framesPerSecondToPlayAt;

    timer_->start(secondsToMs(1/framesPerSecond_));

    isPlaying_ = true;
}

/// Pauses the Animation. The frame that it paused at is displayed.
void Animation::pause()
{
    timer_->stop();
    isPlaying_ = false;
}

/// Returns the bounding rect of the Animation.
QRectF Animation::boundingRect() const
{
    return currentPixmap_->boundingRect();
}

/// Returns the current frame number that is being displayed.
/// Returns -1 if the Animation has no frames.
int Animation::currentFrame()
{
    if (pixmaps_.empty())
        return -1;

    return currentFrame_;
}

/// Returns true if the Animation is currently playing. False if its paused.
/// Use play() to play the animation and pause() to pause it.
bool Animation::isPlaying()
{
    return isPlaying_;
}

/// Executed periodically to make the Animation display the next frame.
void Animation::animationStep_()
{
    // if we have played enough times, stop playing
    if (timesPlayed_ >= timesToPlay_ && timesToPlay_ != -1){
        emit finished(this);
        pause();
        return;
    }

    // if there are no more frames, start again and increment times played
    if (currentFrame_ >= pixmaps_.size()){
        emit finished(this);
        currentFrame_ = 0;
        ++timesPlayed_;
        return;
    }

    // show the next frame
    currentPixmap_->setPixmap(pixmaps_[currentFrame_]);
    ++currentFrame_;
}

/// Common code for ctors.
/// Basically sets up the Animation to a valid initial state.
void Animation::initialize_()
{
    pixmaps_ = std::vector<QPixmap>();
    currentPixmap_ = new QGraphicsPixmapItem(this);
    timer_ = new QTimer(this);
    isPlaying_ = false;
    currentFrame_ = 0;
    timesPlayed_ = 0;
    timesToPlay_ = 0;
    framesPerSecond_ = 0;

    connect(timer_,&QTimer::timeout,this,&Animation::animationStep_);
}

void Animation::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // do nothing
}
