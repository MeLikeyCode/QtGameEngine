#include "EntitySprite.h"

#include <QRectF>
#include <QGraphicsItem>

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

/// Sets the size of the EntitySprite to be a scaled version of its current size.
/// @param value Can be from 0-1. A value of 0.5 means shrink in half, a value of 2 means double size.
void EntitySprite::scale(double value)
{
    QRectF bbox = boundingBox();
    double newWidth = bbox.width() * value;
    double newHeight = bbox.height() * value;
    setSize(QSize(newWidth,newHeight));
}
