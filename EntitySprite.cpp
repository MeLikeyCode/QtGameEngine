#include "EntitySprite.h"

#include <QRectF>
#include <QGraphicsItem>

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
