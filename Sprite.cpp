#include "Sprite.h"
#include <cassert>
#include <QDebug>
#include <QGraphicsScene>

Sprite::Sprite(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    // constructor body
    // initialize
    timer_ = new QTimer(this);
    pixmap_ = new QGraphicsPixmapItem((QPixmap(":resources/graphics/misc/defaultEntity.png")),this);
    currentFrame_ = 0;
    timesPlayed_ = 0;
    timesToPlay_ = 0;

    // origin
    setTransformOriginPoint(pixmap_->boundingRect().width()/2,
                            pixmap_->boundingRect().height()/2);
}

/// Plays the specified animation the specified number of times with the specified
/// delay between each frame.
void Sprite::play(std::string animation, int timesToPlay, int delayBetweenFrames){
    // make sure the animation exists
    assert(animation_.find(animation) != animation_.end());

    // stop the currently playing animation
    stop();

    // set up to play this animation
    playingAnimation_ = animation;
    animationPixmaps = animation_[animation];
    timesPlayed_ = 0;
    currentFrame_ = 0;
    timesToPlay_ = timesToPlay;

    // play the animation
    connect(timer_,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer_->start(delayBetweenFrames);

}

QRectF Sprite::boundingRect() const{
    return pixmap_->boundingRect();
}

/// Returns true if the Sprite contains the specified animation.
bool Sprite::hasAnimation(std::string animation) const{
    return animation_.find(animation) != animation_.end();
}

/// Returns the name of the currently playing animation.
std::string Sprite::playingAnimation()
{
    return playingAnimation_;
}

/// Adds the specified pixmap as a frame to the specified animation.
///
/// If the animation already exists, the pixmap will simply be added as the next
/// frame in the animation. If the animation does not exist, it will be created with
/// the pixmap being its first frame.
void Sprite::addFrame(QPixmap frame, std::string toAnimation){
    // if the anim already exists, simply push back the new pixmap into
    // the anim's vector
    if (animation_.find(toAnimation) != animation_.end()){
        animation_[toAnimation].push_back(frame);
    }

    // if the anim does not exist, create it and add push back the pixmap
    // as the first pixmap in the anim
    else {
        animation_[toAnimation] = std::vector<QPixmap>();
        animation_[toAnimation].push_back(frame);
    }

}

/// Adds the _.png_ images in the specified resource folder with the specified
/// image name prefix as frames of a newly created animation that has the same
/// name as the image prefix.
///
/// You need to tell this function how many images with the specified prefix are
/// in the specified resource folder. The actual names of the images in the
/// resource folder should be the prefix with a number attached. The number
/// should start at 0 and go up.
///
/// Example:
/// Sprite* sprite = new Sprite();
/// sprite->addPixmaps(":graphics/player/walk",9,"walkdown");
/// sprite->play("walkdown",100,50);
///
/// The above example will go inside :graphics/player/walk resource folder and
/// add walkdown0, walkdown1, walkdown2...all the way to walkdown8 as frames for an
/// animation called "walkdown".
void Sprite::addFrames(std::string resourceFolder, int numOfImages, std::string imagePrefix){
    for (int i = 0, n = numOfImages; i < n; ++i){
        std::string fullPath = resourceFolder + "/" + imagePrefix + std::to_string(i) + ".png";
        addFrame(QPixmap(fullPath.c_str()),imagePrefix);
    }
}

/// Sets the specified pixmap as the one currently being displayed by the Sprite.
void Sprite::setPixmap(QPixmap pixmap){
    pixmap_->setPixmap(pixmap);
}

/// Stops the currently playing animation.
void Sprite::stop(){
    playingAnimation_ = std::string("none");
    timer_->disconnect();
}

void Sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // does nothing because Sprite doesn't need to paint anything,
    // the currently displayed pixmap is painted via a QGraphicsPixmapItem
}

/// Displays the next frame of the current animation. Do not use. It is only
/// meant to be used by the play() method.
void Sprite::nextFrame(){
    // if we have played it enough times and we are not supposed to play it
    // forever, stop
    if (timesPlayed_ >= timesToPlay_ && timesToPlay_ != -1){
        stop();
        return;
    }

    // if there are no more frames, start again and increment times played
    if (currentFrame_ >= animationPixmaps.size()){
        currentFrame_ = 0;
        ++timesPlayed_;
    }

    setPixmap(animationPixmaps[currentFrame_]);
    ++currentFrame_;

}
