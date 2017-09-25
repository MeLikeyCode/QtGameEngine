#include "TopDownSprite.h"

#include "Sprite.h"
#include "SpriteSheet.h"
#include "Node.h"

/// Constructs a TopDownSprite that will have no animations but the frame will by default display a
/// "Default" graphic.
TopDownSprite::TopDownSprite(): sprite_(new Sprite())
{
    underlyingItem_ = sprite_;
}

/// Constructs a TopDownSprite that will have no animations but the specified pixmap will be displayed
/// as the currently showing frame.
TopDownSprite::TopDownSprite(const QPixmap &pixmap)
{
    sprite_ = new Sprite(pixmap);
    underlyingItem_ = sprite_;
}

/// Adds an animation to the TopDownSprite from frames of a SpriteSheet.
/// @param animationName What to call the animation.
/// @param fromSpriteSheet The SpriteSheet to add frames from.
/// @param from Where on the SpriteSheet to start adding frames from.
/// @param to Where on the SpriteSheet to stop adding frames from.
void TopDownSprite::addAnimation(std::string animationName, const SpriteSheet &fromSpriteSheet, const Node &from, const Node &to)
{
    sprite_->addFrames(from,to,fromSpriteSheet,animationName);
}

void TopDownSprite::addAnimation(std::string resourceFolder, int numOfImages, std::string imagePrefix)
{
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

void TopDownSprite::play_(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt)
{
    sprite_->play(animationName,numTimesToPlay,fpsToPlayAt);
}

void TopDownSprite::stop_()
{
    sprite_->stop();
}

QPixmap TopDownSprite::currentlyDisplayedFrame() const
{
    return sprite_->currentFrame();
}

void TopDownSprite::setFacingAngle_(double angle)
{
    // rotate the underlying frames
    sprite_->setRotation(angle);
}
