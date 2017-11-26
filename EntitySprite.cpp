#include "EntitySprite.h"

#include <QRectF>
#include <QGraphicsItem>
#include <cassert>

EntitySprite::EntitySprite(): underlyingItem_(nullptr)
{

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
    playingBeforePlayOnce_ = playingAnimation();
    connect(this,&EntitySprite::animationFinished,this,&EntitySprite::onPlayThenGoBackDone_);
    play(animationToPlay,numTimesToPlay,fpsToPlayAt,startingFrameNumber);
}

void EntitySprite::scale(double scale)
{
    underlyingItem_->setScale(scale);
}

void EntitySprite::onPlayThenGoBackDone_()
{
    disconnect(this,&EntitySprite::animationFinished,this,&EntitySprite::onPlayThenGoBackDone_);
    play(playingBeforePlayOnce_.name(),playingBeforePlayOnce_.timesLeftToPlay(),playingBeforePlayOnce_.fps(),playingBeforePlayOnce_.currentFrame());
}

void EntitySprite::setOrigin(const QPointF &pos)
{
    pos_ = pos;
    assert(underlyingItem_);
    underlyingItem_->setTransformOriginPoint(pos);
}

QPointF EntitySprite::origin() const
{
    return pos_;
}
