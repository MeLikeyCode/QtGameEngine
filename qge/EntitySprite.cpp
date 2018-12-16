#include "EntitySprite.h"

using namespace qge;

EntitySprite::EntitySprite(): underlyingItem_(nullptr)
{
    // empty
}

EntitySprite::~EntitySprite()
{
    delete underlyingItem_;
}

void EntitySprite::setFacingAngle(double angle)
{
    facingAngle_ = angle;
    setFacingAngle_(angle);
}

double EntitySprite::facingAngle() const
{
    return facingAngle_;
}

QRectF EntitySprite::boundingBox() const
{
    // Implementation Details:
    // - by default, return the bounding box of the underlying QGraphicsItem
    // - is virtual, so that sub classes can provide a better implementation if they wish

    return underlyingItem_->boundingRect();
}

/// Plays the specified animation, then goes back to w.e. animation was playing previously.
void EntitySprite::playThenGoBackToOldAnimation(const std::string animationToPlay, int numTimesToPlay, int fpsToPlayAt, int startingFrameNumber)
{
    animationPlayingBefore_ = playingAnimation();
    connect(this,&EntitySprite::animationFinishedCompletely,this,&EntitySprite::onTemporaryAnimationDone_);
    play(animationToPlay,numTimesToPlay,fpsToPlayAt,startingFrameNumber);
}

void EntitySprite::scale(double scale)
{
    underlyingItem_->setScale(scale);
}

void EntitySprite::onTemporaryAnimationDone_(EntitySprite *sender, std::string animation)
{
    if (animationPlayingBefore_.isNone() == false){
        disconnect(this,&EntitySprite::animationFinishedCompletely,this,&EntitySprite::onTemporaryAnimationDone_);
        play(animationPlayingBefore_.name(),animationPlayingBefore_.timesLeftToPlay(),animationPlayingBefore_.fps(),animationPlayingBefore_.currentFrame());
    }
}
