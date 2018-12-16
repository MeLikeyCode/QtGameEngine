#include "TopDownSprite.h"

#include "Sprite.h"
#include "SpriteSheet.h"
#include "Node.h"

using namespace qge;

/// Constructs a TopDownSprite that will have no animations but the frame will by default display a
/// "Default" graphic.
TopDownSprite::TopDownSprite(): sprite_(new Sprite())
{
    commonInitialize_();
}

/// Constructs a TopDownSprite that will have no animations but the specified pixmap will be displayed
/// as the currently showing frame.
TopDownSprite::TopDownSprite(const QPixmap &pixmap): sprite_(new Sprite(pixmap))
{
    commonInitialize_();
}

/// Adds frames from a SpriteSheet to the TopDownSprite.
/// @param animationName Name of the animation.
/// @param fromSpriteSheet The SpriteSheet to add frames from.
/// @param from Where on the SpriteSheet to start adding frames from.
/// @param to Where on the SpriteSheet to stop adding frames from.
/// If the animation with the specified name doesn't exit, it will be created.
void TopDownSprite::addFrames(std::string animationName, const SpriteSheet &fromSpriteSheet, const Node &from, const Node &to)
{
    bool hasDigits = std::find_if(std::begin(animationName),std::end(animationName),[](char c){return isdigit(c);}) != std::end(animationName);
    assert(!hasDigits); // numbers not allowed in animation names

    sprite_->addFrames(from,to,fromSpriteSheet,animationName);
}

/// Another way to add frames to an animation (from a set of images with the same prefix and
/// incrementing postfix numbers in the same folder). See addAnimation(std::string, const
/// SpriteSheet&, const Node&, const Node&) for more info.
void TopDownSprite::addFrames(std::string resourceFolder, int numOfImages, std::string imagePrefix)
{
    bool hasDigits = std::find_if(std::begin(imagePrefix),std::end(imagePrefix),[](char c){return isdigit(c);}) != std::end(imagePrefix);
    assert(!hasDigits); // numbers not allowed in animation names

    sprite_->addFrames(resourceFolder,numOfImages,imagePrefix,imagePrefix);
}

/// Adds a single frame to the TopDownSprite.
/// If the specified animation doesn't exist, it will be created.
void TopDownSprite::addFrame(const QPixmap &frame, const std::string &toAnimation)
{
    sprite_->addFrame(frame,toAnimation);
}

double TopDownSprite::actualFacingAngle() const
{
    // the actual facing angle of a TopDownSprite will always be the facing angle
    // (since we simply rotate the pictures to face exactly where we are asked to)

    return facingAngle();
}

QRectF TopDownSprite::boundingBox() const
{
    return sprite_->boundingRect();
}

bool TopDownSprite::hasAnimation(const std::string &animationName) const
{
    return sprite_->hasAnimation(animationName);
}

void TopDownSprite::play(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt, int startingFrameNumber)
{
    sprite_->play(animationName,numTimesToPlay,fpsToPlayAt,startingFrameNumber);
}

void TopDownSprite::stop()
{
    sprite_->stop();
}

PlayingAnimationInfo TopDownSprite::playingAnimation()
{
    return sprite_->playingAnimation();
}

void TopDownSprite::commonInitialize_()
{
    underlyingItem_ = sprite_;

    // listen to when internal sprite finishes playing animations
    connect(sprite_,&Sprite::animationFinished,this,&TopDownSprite::onInternalSpriteAnimationFinished_);
    connect(sprite_,&Sprite::animationFinishedCompletely,this,&TopDownSprite::onInternalSpriteAnimationCompletelyFinished_);
    connect(sprite_,&Sprite::frameSwitched,this,&TopDownSprite::onInternalSpriteFrameSwitched_);
}

QPixmap TopDownSprite::currentlyDisplayedFrame() const
{
    return sprite_->currentFrame();
}

/// Executed when the underlying sprite emits the "animationFinished" signal.
/// Will emit our own animationFinished signal in response.
void TopDownSprite::onInternalSpriteAnimationFinished_(Sprite *sender, std::string animation)
{
    emit animationFinished(this,animation);
}

/// Executed when the underlying sprite emits the "animationFinishedCompletely" signal.
/// Will emit our own animationFinishedComplete signal in response.
void TopDownSprite::onInternalSpriteAnimationCompletelyFinished_(Sprite *sender, std::string animation)
{
    emit animationFinishedCompletely(this,animation);
}

void TopDownSprite::onInternalSpriteFrameSwitched_(Sprite *sender, int fromFrameNum, int toFrameNum)
{
    emit frameSwitched(this,fromFrameNum,toFrameNum);
}

void TopDownSprite::setFacingAngle_(double angle)
{
    // rotate the underlying frames
    sprite_->setRotation(angle);
}
