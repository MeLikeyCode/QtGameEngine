#include "EntitySprite.h"

#include <QRectF>
#include <QGraphicsItem>

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

void EntitySprite::scale(double scale)
{
    underlyingItem_->setScale(scale);
}

void EntitySprite::play(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt)
{
    play_(animationName,numTimesToPlay,fpsToPlayAt); // delegate to concrete classes
    playingAnimation_ = animationName; // remember that we are currently playing this animation
}

std::string EntitySprite::playingAnimation() const
{
    return playingAnimation_;
}

void EntitySprite::stop()
{
    stop_(); // delegate to sub class
    playingAnimation_ = ""; // remember that we are not currently playing any animations
}
