#include "Sprite.h"

#include "Node.h"
#include "SpriteSheet.h"
#include "Utilities.h"
#include "PlayingAnimationInfo.h"

using namespace qge;

/// Constructs a Sprite with a default displayed frame.
Sprite::Sprite(QGraphicsItem *parent): QGraphicsItem(parent)
{
    pixmapItem_ = new QGraphicsPixmapItem((QPixmap(":resources/graphics/misc/defaultEntity.png")),this);
    commonInitialize_();
}

/// Constructs a Sprite with the specified pixmap being the currently displayed frame.
Sprite::Sprite(QPixmap pixmap, QGraphicsItem *parent): QGraphicsItem(parent)
{
    pixmapItem_ = new QGraphicsPixmapItem(pixmap,this);
    commonInitialize_();
}

/// Plays the specified animation the specified number of times at the specified fps.
/// Optionally, you specify the frame number to start playing at.
/// Does nothing if the specified animation is already playing. If you want to replay
/// a currently playing animiation then you need to call stop() and then play().
/// If any other animations are currently playing, they will be stopped immediately and
/// the specified animation will start playing.
void Sprite::play(std::string animation, int timesToPlay, double framesPerSecond, int startingFrameNumber){
    // make sure the animation exists
    assert(animation_.find(animation) != animation_.end());

    // do nothing if we're already playing the specified animation
    if (playingAnimation().name() == animation)
        return;

    // stop the currently playing animation
    stop();

    // set up to play this animation
    playingAnimation_ = animation;
    playingAnimationFPS_ = framesPerSecond;
    animationPixmaps = animation_[animation];
    timesPlayed_ = 0;
    currentFrame_ = startingFrameNumber;
    timesToPlay_ = timesToPlay;

    // play the animation
    nextFrame_();   // go to next frame immediately
    connect(timer_,SIGNAL(timeout()),this,SLOT(nextFrame_())); // keep going to
    timer_->start(secondsToMs(1/framesPerSecond));                         // next frames
}

/// Plays an animation and then goes back to whatever (if any) animation was previously playing.
void Sprite::playThenGoBackToOldAnimation(std::string animation, int numTimesToPlay, double framesPerSecond, int startingFrameNumber)
{
    animationPlayingLast_ = playingAnimation();
    disconnect(this,&Sprite::animationFinishedCompletely,this,&Sprite::onTemporaryPlayDone_); // prevent double connect
    connect(this,&Sprite::animationFinishedCompletely,this,&Sprite::onTemporaryPlayDone_);
    play(animation,numTimesToPlay,framesPerSecond,startingFrameNumber);
}

QRectF Sprite::boundingRect() const{
    return pixmapItem_->boundingRect();
}

/// Returns true if the Sprite contains the specified animation.
bool Sprite::hasAnimation(std::string animation) const{
    return animation_.find(animation) != animation_.end();
}

/// Returns the names of all the animations this Sprite has.
std::vector<std::string> Sprite::animations() const
{
    std::vector<std::string> results;
    for (const auto& animationPixmapPair : animation_)
        results.push_back(animationPixmapPair.first);
    return results;
}

/// Returns the currently playing animation.
/// PlayingAnimationInfo::isNone() will return true if no animation is currently playing.
PlayingAnimationInfo Sprite::playingAnimation() const
{
    PlayingAnimationInfo info(playingAnimation_,playingAnimationFPS_,playingAnimationTimesLeftToPlay_(),currentFrameNumber());
    return info;
}

/// Returns the number of times we have left to play the currently playing animation.
/// Sprite::play() allows you to pass in the number of times you want the animation to be played.
/// This function can tell you how many times you have left to play.
/// Returns -1 if the animation was asked to play an infinite number of times (thus it has an infinite number of times left to play).
/// Includes the current run.
int Sprite::playingAnimationTimesLeftToPlay_() const
{
    if (timesToPlay_ == -1)
        return -1;

    return timesToPlay_ - timesPlayed_;
}

/// Returns the currently displayed frame of the Sprite.
QPixmap Sprite::currentFrame() const
{
    return pixmapItem_->pixmap();
}

/// Returns the frame number of the currently displayed frame.
/// Frame numbers start at 0 for animations.
int Sprite::currentFrameNumber() const
{
    return currentFrame_;
}

/// Adds the specified pixmap as a frame to the specified animation.
///
/// If the animation already exists, the pixmap will simply be added as the next
/// frame in the animation. If the animation does not exist, it will be created with
/// the pixmap being its first frame.
void Sprite::addFrame(QPixmap frame, std::string toAnimation){
    assert(toAnimation != "");

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

/// Adds a bunch of images (.png) in a folder as frames of an animation.
///
/// You need to tell this function how many images with the specified prefix are
/// in the specified folder. The actual names of the images in the
/// resource folder should be the prefix with a number attached. The number
/// should start at 0 and go up to n-1 (n being the number of images in with
/// the specified prefix in the folder).
///
/// Example:
/// Sprite* sprite = new Sprite();
/// sprite->addPixmaps(":resources/graphics/player/walk",9,"walkdown");
/// sprite->play("walkdown",100,50);
///
/// The above example will go inside ':/resources:graphics/player/walk' folder and
/// add walkdown0, walkdown1, walkdown2...all the way to walkdown8 as frames for an
/// animation called "walkdown".
void Sprite::addFrames(std::string folder, int numOfImages, const std::string &imagePrefix, const std::string &toAnimation){
    for (int i = 0, n = numOfImages; i < n; ++i){
        std::string fullPath = folder + "/" + imagePrefix + std::to_string(i) + ".png";
        addFrame(QPixmap(fullPath.c_str()),toAnimation);
    }
}

/// Adds a tile from a SpriteSheet to an animation of the Sprite.
///
/// If the animation already exists, the tile will simply be added as the next
/// frame in the animation. If the animation does not exist, it will be created with
/// the tile being its first frame.
void Sprite::addFrame(const Node &tile, const SpriteSheet &fromSpriteSheet, std::string toAnimation)
{
    QPixmap asPixmap = fromSpriteSheet.tileAt(tile);
    addFrame(asPixmap,toAnimation);
}

/// Adds a specified set of tiles from a SpriteSheet to an animation of the Sprite.
///
/// If the animation already exists, the tiles will simply be added as subsequent
/// frames in the animation. If the animation does not exist, it will be created first.
void Sprite::addFrames(const Node &topLeftCell, const Node &bottomRightCell, const SpriteSheet &fromSpriteSheet, std::string toAnimation)
{
    std::vector<QPixmap> pixmaps = fromSpriteSheet.tilesAt(topLeftCell,bottomRightCell);
    for (QPixmap pixmap:pixmaps){
        addFrame(pixmap,toAnimation);
    }
}

/// Sets the specified pixmap as the one currently being displayed by the Sprite.
void Sprite::setPixmap(QPixmap pixmap){
    pixmapItem_->setPixmap(pixmap);
}

/// Stops the currently playing animation.
void Sprite::stop(){
    playingAnimation_ = "";
    playingAnimationFPS_ = -1;
    timer_->disconnect();
}

void Sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    // does nothing because Sprite doesn't need to paint anything,
    // the currently displayed pixmap is painted via a QGraphicsPixmapItem
}

/// Displays the next frame of the current animation. Do not use. It is only
/// meant to be used by the play() method.
void Sprite::nextFrame_(){
    // if we have played it enough times and we are not supposed to play it
    // forever, stop
    if (timesPlayed_ >= timesToPlay_ && timesToPlay_ != -1){
        emit animationFinished(this,playingAnimation().name());
        emit animationFinishedCompletely(this,playingAnimation().name());
        stop();
        return;
    }

    // if there are no more frames, start again and increment times played
    if (currentFrame_ >= animationPixmaps.size()){
        emit animationFinished(this,playingAnimation().name());
        currentFrame_ = 0;
        ++timesPlayed_;
    }

    setPixmap(animationPixmaps[currentFrame_]);
    emit frameSwitched(this,currentFrame_ == 0 ? animationPixmaps.size() - 1 : currentFrame_ - 1, currentFrame_);
    ++currentFrame_;

}

/// Executed when a "temporary" play has just finished.
/// A temporary play initiates due to playThenGoBackToOldAnimation().
/// Will play the old playing animation.
void Sprite::onTemporaryPlayDone_(Sprite *sender, std::string animation)
{
    // client takes care of preventing double connects

    if (animationPlayingLast_.isNone() == false)
        play(animationPlayingLast_.name(),animationPlayingLast_.timesLeftToPlay(),animationPlayingLast_.fps(),animationPlayingLast_.currentFrame());
}

/// Contains common initialization code for several ctors (reduces code duplication).
void Sprite::commonInitialize_()
{
    timer_ = new QTimer(this);
    currentFrame_ = 0;
    timesPlayed_ = 0;
    timesToPlay_ = 0;

    playingAnimation_ = "";
    playingAnimationFPS_ = -1;
}
