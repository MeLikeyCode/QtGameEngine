#include "Animation.h"

#include "SpriteSheet.h"
#include "Utilities.h"
#include "Node.h"

using namespace qge;

/// Constructs an Animation with no frames.
/// Use addFrame() or addFrames() to add some frames.
Animation::Animation(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();
}

/// Constructs an Animation with just 1 frame.
/// Use addFrame() or addFrames() to add additional frames.
Animation::Animation(const QPixmap &pixmap, QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();

    addFrame(pixmap);
    currentPixmap_->setPixmap(pixmap); // display frame right away
    currentPixmap_->setPixmap(pixmap);
}

/// Constructs an Animation with the specified sequence of frames from the
/// specified SpriteSheet.
Animation::Animation(const SpriteSheet &spriteSheet, Node from, Node to, QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    initialize_();

    addFrames(spriteSheet,from,to);
    if (!pixmaps_.empty())
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

/// Goes inside the specified folder ('folderPath') and looks for images with the following pattern:
/// "imagePrefix0.fileExtension"
/// "imagePrefix1.fileExtension"
/// "imagePrefix2.fileExtension"
/// Will look for images all the way up to but not including 'numOfImages'.
///
/// It will load each of these images as frames of the Animation.
/// This function is useful if each of your frames are a sperate image. Simply
/// place all the images in one folder, give them all the same prefix (and file extension), and
/// number them from 0 and up.
///
/// Example
/// =======
/// If your folder structure looks like so:
/// C:/myfolder
///     walk0.png
///     walk1.png
///     walk2.png
/// You can load these images as frames of an Animation by the following addFrames() call:
/// addFrames("C:/myfolder",3,"walk",".png");
void Animation::addFrames(std::string folderPath, int numOfImages, std::string imagePrefix, std::string fileExtension)
{
    for (int i = 0, n = numOfImages; i < n; ++i){
        std::string fullPath = folderPath + "/" + imagePrefix + std::to_string(i) + fileExtension;
        addFrame(QPixmap(fullPath.c_str()));
    }
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

    // remove static frame (if there)
    currentPixmap_->setPixmap(QPixmap());

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
QPixmap Animation::currentFrame()
{
    if (pixmaps_.empty())
        return QPixmap();

    return currentPixmap_->pixmap();
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
