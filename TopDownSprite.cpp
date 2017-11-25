#include "TopDownSprite.h"

#include <cassert>


#include "Sprite.h"
#include "SpriteSheet.h"
#include "Node.h"

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

/// Adds an animation to the TopDownSprite from frames of a SpriteSheet.
/// @param animationName What to call the animation.
/// @param fromSpriteSheet The SpriteSheet to add frames from.
/// @param from Where on the SpriteSheet to start adding frames from.
/// @param to Where on the SpriteSheet to stop adding frames from.
void TopDownSprite::addAnimation(std::string animationName, const SpriteSheet &fromSpriteSheet, const Node &from, const Node &to)
{
    bool hasDigits = std::find_if(std::begin(animationName),std::end(animationName),[](char c){return isdigit(c);}) != std::end(animationName);
    assert(!hasDigits); // numbers not allowed in animation names

    sprite_->addFrames(from,to,fromSpriteSheet,animationName);
}

void TopDownSprite::addAnimation(std::string resourceFolder, int numOfImages, std::string imagePrefix)
{
    bool hasDigits = std::find_if(std::begin(imagePrefix),std::end(imagePrefix),[](char c){return isdigit(c);}) != std::end(imagePrefix);
    assert(!hasDigits); // numbers not allowed in animation names

    sprite_->addFrames(resourceFolder,numOfImages,imagePrefix);
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
    return PlayingAnimationInfo(sprite_->playingAnimation(),
                                sprite_->playingAnimationFPS(),
                                sprite_->playingAnimationTimesLeftToPlay(),
                                sprite_->currentFrameNumber());
}

void TopDownSprite::commonInitialize_()
{
    underlyingItem_ = sprite_;

    // listen to when internal sprite finishes playing animations
    connect(sprite_,&Sprite::animationFinished,this,&TopDownSprite::onInternalSpriteAnimationFinished_);
    connect(sprite_,&Sprite::animationFinishedCompletely,this,&TopDownSprite::onInternalSpriteAnimationCompletelyFinished_);

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

void TopDownSprite::setFacingAngle_(double angle)
{
    // rotate the underlying frames
    sprite_->setRotation(angle);
}
